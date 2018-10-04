# redatam4r

A través de este proyecto se pretende:
 * Completar la especificación del formato binario REDATAM utilizado por el software privativo homónimo de la CEPAL
 * Desarrollar un paquete para el lenguaje R que permita la carga de bases de datos censales para su análisis estadístico

### Compilación

```sh
$ make
```

### Uso

Actualmente el ejecutable lee e interpreta el archivo de diccionario casi en su totalidad
y escribe los metadatos en la salida estándar:

```sh
$ redatam4r [path_al_archivo_diccionario.dic]
```

### Notas

En este repositorio no se distribuyen ni se divulgarán las bases de datos REDATAM de
ningún organismo estadístico.

En este repositorio no se distribuyen ni se divulgarán datos que estén amparados
bajo la ley de secreto estadístico.

### Referencias

De Grande, P. *El Formato REDATAM*. Estudios Demográficos y Urbanos (2015) [(PDF)](https://estudiosdemograficosyurbanos.colmex.mx/index.php/edu/article/view/15/pdf)

