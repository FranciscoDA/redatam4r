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

Las entidades de la base de datos se representan como objetos de clase `redatam.entity`
y además heredan a `data.frame`. Los objetos `redatam.entity` contienen atributos
especiales con metadatos de las entidades. Las columnas del dataframe serán
las variables de la entidad, y cada fila será una instancia de esa entidad. La
primer columna es especial en el sentido de que se corresponde con el número de
fila de la entidad de nivel superior.

Todas las variables son una columna dentro del dataframe de la entidad que las
contiene. La clase del vector de una variable es `redatam.variable`.

Algunos metadatos de los objetos de entidades y variables pueden ser consultados
mediante los métodos `description` y `documentation`.

Ejemplo de uso:
```
> library(redatam4r)
> a <- read.redatam('/path/al/diccionario.dic')
> a$DPTO

 PROV_id var1 var2 var3
1      1    1    1    1
2      1    2    4    9
3      1    1   20    3
...

> description(a$DPTO)
[1] "Departamento / Partido"

> description(a$HOGAR$H23)
[1] "Antiguedad de la vivienda"
```

### Notas

En este repositorio no se distribuyen ni se divulgarán las bases de datos REDATAM de
ningún organismo estadístico.

En este repositorio no se distribuyen ni se divulgarán datos que estén amparados
bajo la ley de secreto estadístico.

### Referencias

De Grande, P. *El Formato REDATAM*. Estudios Demográficos y Urbanos (2015) [(PDF)](https://estudiosdemograficosyurbanos.colmex.mx/index.php/edu/article/view/15/pdf)

