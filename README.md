# redatam4r

A través de este proyecto se pretende:
 * Completar la especificación del formato binario REDATAM utilizado por el software privativo homónimo de la CEPAL
 * Desarrollar un paquete para el lenguaje R que permita la carga de bases de datos censales para su análisis estadístico

### Compilación

Para compilar el paquete se debe ejecutar el siguiente comando desde el directorio superior:
```sh
$ R CMD INSTALL redatam4r
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
```
> library(redatam4r)
> a <- read.redatam('/path/al/diccionario.dic')
> head(a$DPTO)

 PROV_id var1 var2 var3
1      1    1    1    1
2      1    2    4    9
3      1    1   20    3
...

> description(a$DPTO)
> description(a$HOGAR$H23)

> h2417 = a$HOGAR$H2417[1:length(a$HOGAR$H2417)]
# la variable h2417 indica si el hogar es alquilado o propiedad del encuestado
> h_alq = h2417=="Alquilada"
> h_pro = h2417=="Propia"

> p_alq = a$DPTO$PROV_id[a$VIVIENDA$DPTO_id[a$HOGAR$VIVIENDA_id[h_alq]]]
> p_pro = a$DPTO$PROV_id[a$VIVIENDA$DPTO_id[a$HOGAR$VIVIENDA_id[h_pro]]]

> df = data.frame(
	inquilinos=tabulate(p_alq),
	propietarios=tabulate(p_pro),
	provincia=head(a$PROV$NOMPROV, 24)
)
```

Distribución de hogares con propietario por provincia:
`> barplot(df$propietarios, names.arg=df$provincia, las=2)`
![grafico propietarios](https://i.imgur.com/oEXEdiR.png)

Distribución de hogares con inquilinos por provincia:
`> barplot(df$inquilinos, names.arg=df$provincia, las=2)`
![grafico inquilinos](https://i.imgur.com/nkm9IP4.png)

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

