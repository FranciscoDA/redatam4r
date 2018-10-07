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

La única función expuesta en el paquete es `read.redatam`, la cual acepta un path
al archivo diccionario de la base de datos. La función devuelve un objeto
cuyos nombres de claves serán los identificadores de las entidades. Cada miembro
del objeto raíz es, a su vez, un data.frame que contiene cada variable de la entidad
en sus columnas. Se agrega, además, una columna adicional para indicar el número
de fila de la instancia de la entidad padre dentro del dataframe correspondiente.
```
> a <- read.redatam('/path/al/diccionario.dic')
> a$DPTO

 PROV_id var1 var2 var3
1      1    1    1    1
2      1    2    4    9
3      1    1   20    3
...
```

### Notas

En este repositorio no se distribuyen ni se divulgarán las bases de datos REDATAM de
ningún organismo estadístico.

En este repositorio no se distribuyen ni se divulgarán datos que estén amparados
bajo la ley de secreto estadístico.

### Referencias

De Grande, P. *El Formato REDATAM*. Estudios Demográficos y Urbanos (2015) [(PDF)](https://estudiosdemograficosyurbanos.colmex.mx/index.php/edu/article/view/15/pdf)

