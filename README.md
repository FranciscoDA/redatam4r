# redatam4r

A través de este proyecto se pretende:
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

La función principal expuesta en el paquete es `read.redatam`, la cual acepta un path
al archivo diccionario de la base de datos.

`read.redatam` devuelve un objeto de clase `redatam.database`. Los elementos nombrados
de este objeto serán las entidades de la base de datos. Los nombres de los elementos
serán sus identificadores dentro de la base de datos.

Las _entidades_ de la base de datos se representan como objetos de clase `redatam.entity`
y además heredan a `data.frame`. Las columnas del dataframe serán
las _variables_ de la entidad, y cada fila será un _elemento_ de dicha entidad. La
primer columna del dataframe es especial, puesto que indica el número de
fila del elemento padre de la entidad de nivel superior.

Todas las variables se almacenan como una columna dentro del dataframe de la entidad a la que pertenecen.
La clase del vector de una variable es `redatam.variable`. Las
observaciones de las variables pueden representar un gran volumen de
datos, por lo que trabajar con todas las variables en memoria
se volvería una tarea demandante. Es por esto que la carga de las observaciones
se realiza sólo mediante el operador de extracción (`[`), en caso contrario
los vectores permanecen vacíos.

Algunos metadatos de los objetos `redatam.database`, `redatam.entity` y `redatam.variable` pueden ser consultados
mediante el método `description`.

### Ejemplo:
```r
library(redatam4r)
a <- read.redatam('/path/al/diccionario.dic')
head(a$DPTO)
```
```
 PROV_id var1 var2 var3
1      1    1    1    1
2      1    2    4    9
3      1    1   20    3
...
```
```r
description(a$DPTO)
description(a$HOGAR$H23)

h2417 = a$HOGAR$H2417[]

# la variable h2417 indica si el hogar es alquilado o propiedad del encuestado
# filtrar los elementos que deseamos (hogares alquilados y propios)
h_alq = h2417=="Alquilada"
h_pro = h2417=="Propia"

# obtener los id de provincia a partir de los id de vivienda
p_alq = a$DPTO$PROV_id[a$VIVIENDA$DPTO_id[a$HOGAR$VIVIENDA_id[h_alq]]]
p_pro = a$DPTO$PROV_id[a$VIVIENDA$DPTO_id[a$HOGAR$VIVIENDA_id[h_pro]]]

# tabular los datos (contar apariciones de cada provincia)
p_alq = tabulate(p_alq)
p_pro = tabulate(p_pro)

# consolidar los datos en un dataframe
df = data.frame(
	inquilinos=p_alq,
	propietarios=p_pro,
	inqxprop=p_alq/p_pro,
	provincia=head(a$PROV$NOMPROV, 24)
)

# graficar barras
midpoints = barplot(df$inqxprop)

# graficar nombres de provincia
text(midpoints, 0, adj=c(0,NA), labels=df$provincia, srt=90)

# titulo y nombres de ejes
title('Cantidad de inquilinos por propietario', xlab='Provincias', ylab='Relación')
```

![grafico\_inq\_x\_prop](https://i.imgur.com/wwKDHso.png)

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

