# MIA-Proyecto1_201712057

## -Manual de Usuario

En esta aplicacion se controla un sistema de archivos y discos para el uso de cualquier persona, contiene una seria de comandos que les ayudara a crear, eliminar discos emulados con extension **.dk** , podra crear particiones en dichos discos creados, de tipo principal y extendidas e incluso logicas.
Podra montar las particiones que creo con un comando simple. Podra tener diferentes usuarios para controlar los archivos y carpetas que quiera tener en dichas particiones.
Y podra obtener reportes graficos con una seria de comandos incluidos.
Tendra opcion de cargar una seria de comandos preestablecidos por medio de un archivo de texto como un script encarga masiva.

### Comandos Soportados por la Aplicacion
* * *
- **MKDISK** :
Este comando creara un archivo binario que simula un disco duro, este archivo binario tendra la extension **.dk**. Su contenido al inicio sera 0 o vacio.
Tendra una serie de parametros los cuales son :

  + **-size**  *obligatorio* : este parametro recibira un numero que indicara el tamaño del disco a crear.
  + **-f** *opcional* : indica el ajuste del disco a crear: **BF, FF, WF** 
  + **-u** *opcional* : letra que representa las unidades para el parametro size. **k,m**
  + **-path** *obligatorio* : sera la ruta donde se creara el archivo binario

- **Ejemplos**
Crea un disco de 200 Mb en la carpeta home
`Mkdisk -size=200 -u=M -path=/home/user/Disco1.dk`
Creara un disco de 10 Mb ya que no hay parametro unit
`Mkdisk -size=10 -path=/home/user/Disco1.dk`
* * *