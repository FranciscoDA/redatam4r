# redatam4r

Los objetivos de este proyecto son:
 * Completar la especificación del formato binario REDATAM utilizado por el software privativo homónimo de la CEPAL
 * Desarrollar una libreria (libredatam) que facilite la carga de bases de datos censales Redatam
 * Desarrollar un paquete (redatam4r) para el lenguaje R basado en libredatam

### Compilación e Instalación

```sh
# compilar libredatam:
$ make
# compilar e instalar redatam4r
$ R CMD INSTALL redatam4r
```
Es necesario que el compilador y R soporten C++17 (para compilar libredatam y redatam4r, respectivamente)

La documentación y el archivo `NAMESPACE` se encuentran incluidos en este repositorio. Si se modifican los archivos en `redatam4r/R` puede que sea necesario regenerarlos con roxygen2. Para esto basta con ejecutar la linea de comandos de R en la raíz del proyecto con los siguientes comandos:
```r
> library(devtools)
> devtools::document('redatam4r')
```

### Uso

La función principal exportada por el paquete es `read.redatam`, la cual acepta un path
al archivo diccionario de la base de datos:


```r
library(redatam4r)
redatamdb <- read.redatam('/windows/Redatam/Censos/CPV2010Ampl/CPV2010Ampliado_Dpto_pub.dic')
```

`read.redatam` devuelve un objeto de clase `redatam.database`. Los elementos nombrados
de este objeto serán las _entidades_ de la base de datos. Los nombres de los elementos
se corresponden con sus identificadores dentro de la base de datos:

```r
names(redatamdb)
```

```
## [1] "PROV"     "DPTO"     "VIVIENDA" "HOGAR"    "PERSONA"
```

La otra función exportada es `description`, la cual imprime una descripción del objeto Redatam que recibe como argumento.
Los datos que mostrará la función dependen del tipo de objeto que se utilizó como argumento.

```r
description(redatamdb)
```

```
## UNK1: 1282
## Name: CPV2010Ampliado_Dpto_pub.dic
## Creation date: 20151102
## Modification date: 20160603
## Root dir: C:\Servers\paises\arg\cgibin\Rpbases\Censos\CPV2010Ampl\BASE_DPTO_Pub\
```

Las _entidades_ de la base de datos se representan como objetos de clase `redatam.entity`
y además heredan a `data.frame`. Las columnas del dataframe serán
las _variables_ de la entidad, y cada fila será un _elemento_ de dicha entidad. La
primer columna del dataframe es especial, puesto que indica el número de
fila del elemento padre de la entidad de nivel superior:


```r
knitr::kable(head(redatamdb$DPTO))
```



| PROV_id|IDDPTO | DPTO|NOMDPTO   |
|-------:|:------|----:|:---------|
|       1|001    | 2001|Comuna 01 |
|       1|002    | 2002|Comuna 02 |
|       1|003    | 2003|Comuna 03 |
|       1|004    | 2004|Comuna 04 |
|       1|005    | 2005|Comuna 05 |
|       1|006    | 2006|Comuna 06 |

Notamos, además, que la descripción para las entidades es distinta que la descripción del diccionario:

```r
description(redatamdb$DPTO)
```

```
## Name1: DPTO
## Name2: DPTO
## Parent: PROV
## Description: Departamento / Partido
## PTR file: "../../../../../../../../Redatam/Censos/CPV2010Ampl/CPV200002.ptr"
## Var ID: IDDPTO
## Var Name: NOMDPTO
## Documentation: 
## Num Vars: 3
## Weighting variable: 
## Label: 
## Selectable: Yes
## UNK2: 0x00 0x00 0x00 
## UNK6: 0x2c 0x76 0xb2 0x02 0x0c 0x00 0x00 0x00 0x54 0xeb 0xc8 0x04 0x0c 0x00 0x00 0x00
```

Todas las variables se almacenan como una columna dentro del dataframe de la entidad a la que pertenecen.
La clase del vector de una variable es `redatam.variable`. Las
observaciones de las variables pueden representar un gran volumen de
datos, por lo que trabajar con todas las variables en memoria simultáneamente resulta una tarea demandante.
Es por esto que la carga de los elementos de las variables desde el disco se realiza sólo cuando se utiliza el operador
de extracción (`[`). En caso contrario los vectores permanecen vacíos.


```r
# explorar la variable
description(redatamdb$HOGAR$H2417)
```

```
## Name: H2417
## Declaration: "../../../../../../../../Redatam/Censos/CPV2010Ampl/CPV200048.rbf" PCK (3)
## Filter: 
## Range: 1 TO 5
## Datatype: INTEGER
## Labels: Propia, Alquilada, Prestada, Cedida por trabajo, Otra situación, 
## Descriptor: DECIMALS 0  MISSING 6  NOTAPPLICABLE 0
## Description: Propiedad de la vivienda
## UNK1: 0
## Documentation: 
## ID: 30
## UNK: 0xd0 0x02 0xce 0x04 0x0c 0x00 0x00 0x00 0xc0 0x82 0xc6 0x04 0x0c 0x00 0x00 0x00
```

```r
# cargar la variable desde el disco
h2417 <- redatamdb$HOGAR$H2417[]

# filtrar los elementos por hogares alquilados y propios
h_alq <- h2417=="Alquilada"
h_pro <- h2417=="Propia"

# obtener los id de provincia a partir de los id de vivienda
p_alq <- redatamdb$DPTO$PROV_id[redatamdb$VIVIENDA$DPTO_id[redatamdb$HOGAR$VIVIENDA_id[h_alq]]]
p_pro <- redatamdb$DPTO$PROV_id[redatamdb$VIVIENDA$DPTO_id[redatamdb$HOGAR$VIVIENDA_id[h_pro]]]

# tabular los datos (contar apariciones de cada provincia)
p_alq <- table(redatamdb$PROV$NOMPROV[p_alq])
p_pro <- table(redatamdb$PROV$NOMPROV[p_pro])

knitr::kable(p_alq / (p_alq + p_pro))
```



|Var1                            |      Freq|
|:-------------------------------|---------:|
|Buenos Aires                    | 0.5237585|
|Catamarca                       | 0.5310846|
|Chaco                           | 0.5297758|
|Chubut                          | 0.5273577|
|Ciudad Autónoma de Buenos Aires | 0.5411628|
|Córdoba                         | 0.5218187|
|Corrientes                      | 0.5203577|
|Entre Ríos                      | 0.5283575|
|Formosa                         | 0.5261930|
|Jujuy                           | 0.5303305|
|La Pampa                        | 0.5270025|
|La Rioja                        | 0.5285757|
|Mendoza                         | 0.5180265|
|Misiones                        | 0.5127756|
|Neuquén                         | 0.5069844|
|Río negro                       | 0.5085929|
|Salta                           | 0.5240467|
|San Juan                        | 0.5199148|
|San Luis                        | 0.5162977|
|Santa Cruz                      | 0.5188774|
|Santa Fe                        | 0.5162244|
|Santiago del Estero             | 0.5119710|
|Tierra del Fuego                | 0.5083006|
|Tucumán                         | 0.5100021|

```r
# graficar
midpoints <- barplot(p_alq / (p_alq + p_pro) * 100, names='')
text(midpoints, 0, adj=c(0,NA), labels=paste0(' ', names(p_alq)), srt=90)
title('% de inquilinos', xlab='Provincias', ylab='Proporción')
```

![plot of chunk unnamed-chunk-6](figure/unnamed-chunk-6-1.png)

### Notas

Por conveniencia y compatibilidad con sistemas Unix, los paths a archivos de índices (PTR) y
de base de datos (RBF) se reescriben tomando el nombre base indicado dentro del diccionario
y el directorio en que se encuentra el diccionario. Por ejemplo, si el diccionario
indica que un archivo se encuentra en el directorio `C:\\Redatam\\datos.rbf`, pero
el diccionario se cargó desde el path `/home/user/dic.dic`, entonces el archivo se buscará con el
path `/home/user/datos.rbf`.

En este repositorio no se distribuyen ni se divulgarán las bases de datos REDATAM de
ningún organismo estadístico.

En este repositorio no se distribuyen ni se divulgarán datos que estén amparados
bajo la ley de secreto estadístico.

### Referencias

De Grande, P. *El Formato REDATAM*. Estudios Demográficos y Urbanos (2015) [(PDF)](https://estudiosdemograficosyurbanos.colmex.mx/index.php/edu/article/view/15/pdf)

