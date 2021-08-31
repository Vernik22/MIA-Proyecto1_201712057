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
- **RMDISK** :
Este comando eliminara un archivo binario que simula un disco duro.
Tendra una serie de parametros los cuales son :

  + **-path** *obligatorio* : sera la ruta donde se creara el archivo binario

- **Ejemplos**
  Elimina Disco1
  `Rmdisk -path=/home/user/Disco1.dk`
* * *
- **FDISK** :
Este comando administra las particiones en el archivo que representa al disco duro. Osea le da formato a la particion deseada o crea una particion. Los parametros *-delete* y *-add* son mutuamente excluyentes.
Tendra una serie de parametros los cuales son :

  + **-size**  *obligatorio* : este parametro recibira un numero que indicara el tamaño de la particion a crear.
  + **-u** *opcional* : recibe una letra que define las unidades que utilizara el parametro size: **B, K, M** .
  + **-path** *obligatorio* : ruta en la que se encuentra el disco al que se le creara la particion.
  + **-type** *opcional* : indica el tipo de particion que se creara **P, E, L**.
  + **-f** *opcional* : indica el ajuste que utilizara la particion **BF, FF, WF**.
  + **-delete** *opcional* : este parametro indica que se eliminara una particion **Fast, Full**.
    + **-name** necesario para *-delete*
    + **-path** necesario para *-delete*
  + **-name** *obligatorio* : indicara el nombre de la particion.
  + **-add** *opcional* : recibe un numero positivo para agregar espacio a la particion o negativo para quitar espacio a la particion.

- **Ejemplos**
  Crea una particion primaria llamada Particion1 de 300kb con el peor ajuste en Disco1.dk
  `fdisk –Size=300 –path=/home/Disco1.dk –name=Particion1`
  Elimina de forma completa una particion llamada Particion1
  `fdisk -name=Particion1 –delete=full -path=/home/Disco1.dk`
  Agrega 1Mb a la particion Particion4 del Disco4.dk
  `fdisk -add=1 -u=M -path="/home/misdiscos/Disco4.dk" -name="Particion 4"`
* * *
- **MOUNT** :
Este comando montara una particion del disco en el sistema e imprimira en consola un resumen de todas las particiones montadas actualmente.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : este parametro recibira la ruta en la que se encuentra el disco que se montara.
  + **-name** *obligatorio* : indica el nombre de la particion a cargar.

- **Ejemplos**
  Monta la particion Part1 en el Disco1
  `mount -path=/home/Disco1.dk -name=Part1`
  Monta la particion Part2 en el Disco3
  `mount -path=/home/Disco3.dk -name=Part2`
* * *
- **UMOUNT** :
Este comando desmonta una particion del sistema, se utilizara el id que se le asigno a la particion al momento de montarla.
Tendra una serie de parametros los cuales son :

  + **-id**  *obligatorio* : especifica el id de la particion que se desmontara.

- **Ejemplos**
  Desmonta la particion con el id 121a
  `umount -id=121a`
  Desmonta la particion con el id 662x
  `umount -id=662x`
* * *
- **MKFS** :
Este comando realiza un formateo completo de la particion, se formateara como ext2 por defecto si en caso el parametro fs no esta definido, Tambien creara un archivo en la raiz llamado users.txt que tendra los usuarios y contraseña del sistema de archivos.
Tendra una serie de parametros los cuales son :

  + **-id**  *obligatorio* : indicara el id que se genero con el comando mount.
  + **-type**  *opcional* : indicara que tipo de formateo se realizara **Fast, Full**.
  + **-fs**  *opcional* : indica el sistema de archivos a formatear **2fs, 3fs**.

- **Ejemplos**
  Realiza un formateo rapido de la particion en el id 581a en ext2
  `mkfs -type=fast -id=581A`
  Realiza un formateo completo de la particion en el id 582a
  `mkfs -id=582A`
* * *
- **LOGIN** :
Este comando se utiliza para iniciar sesion en el sistema. No se puede iniciar otra sesion sin haber hecho un *logout*.
Tendra una serie de parametros los cuales son :

  + **-user**  *obligatorio* : especifica el nombre de usuario que iniciara la sesion.
  + **-pwd**  *obligatorio* : indicara la contraseña del usuario que iniciara la sesion.
  + **-id**  *obligatorio* : indicara el id de la particion montada de la cual van a iniciar sesion.

- **Ejemplos**
  Se loguea en el sistema como usuario root
  `login -usr=root -pwd=123 -id= 582A`
  Debe dar error porque ya hay un usuario logueado
  `login -usr="mi usuario" -pwd="mi pwd" -id= 582A`
* * *
- **LOGOUT** :
Este comando se utiliza para cerrar sesion. Debe haber una sesion iniciada anteriormente.
Tendra una serie de parametros los cuales son :

  + **-user**  *obligatorio* : especifica el nombre de usuario que iniciara la sesion.
  + **-pwd**  *obligatorio* : indicara la contraseña del usuario que iniciara la sesion.
  + **-id**  *obligatorio* : indicara el id de la particion montada de la cual van a iniciar sesion.

- **Ejemplos**
  Termina la sesion del usuario
  `logout`
* * *
- **MKGRP** :
Este comando creara un grupo para los usuarios de la particion y se guardara en el archivo user.txt de la particion.
Tendra una serie de parametros los cuales son :

  + **-name**  *obligatorio* : indicara el nombre que tendra el grupo.

- **Ejemplos**
  Crea el grupo usuarios en la particion de la sesion actual
  `mkgrp -name=usuarios`
  Crea el grupo grupo1 de la sesion actual
  `mkgrp -name="grupo1"`
* * *
- **RMGRP** :
Este comando eliminara un grupo para los usuarios de la particion.
Tendra una serie de parametros los cuales son :

  + **-name**  *obligatorio* : indicara el nombre del grupo a eliminar.

- **Ejemplos**
  Elimina el grupo de usuarios en la particion de la sesion actual
  `rmgrp -name=usuarios`
* * *
- **MKUSR** :
Este comando creara un usuario en la particion.
Tendra una serie de parametros los cuales son :

  + **-usr**  *obligatorio* : indicara el nombre del usuario a crear.
  + **-pwd**  *obligatorio* : indicara la contraseña del usuario.
  + **-grp**  *obligatorio* : indicara el grupo al que pertenecera el usuario.

- **Ejemplos**
  Crea usuario user1 en el grupo 'usuarios'
  `mkusr -usr=user1 -pwd=usuario -grp=usuarios`
* * *
- **RMUSR** :
Este comando elimina un usuario en la particion.
Tendra una serie de parametros los cuales son :

  + **-usr**  *obligatorio* : indicara el nombre del usuario a eliminar.

- **Ejemplos**
  Elimina el usuario user1
  `rmusr -usr=user1`
* * *
- **CHMOD** :
Este comando cambia los permisos de un archivo o carpeta dentro del sistema de archivos, el usuario que cambia los permisos debe ser propietario del archivo.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : indica la ruta en la que se encuentra el archivo o carpeta.
  + **-ugo**  *obligatorio* : indica los permisos que tendran los usuarios *777*.
  + **-r**  *opcional* : indica que el cambio sera recursivo en el caso de carpetas afecta a todos los archivos y carpetas.

- **Ejemplos**
  Cambia los permisos de la carpeta home recursivamente
  `chmod -path=/home -R -ugo=764`
  Cambia los permisos de la carpeta home
  `chmod -path=/home -ugo=777`
* * *
- **TOUCH** :
Este comando permitira crear un archivo.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : sera la ruta para el archivo a crear.
  + **-r**  *opcional* : si no existen las carpetas especificadas en el parametro *-path* se crearan.
  + **-size**  *opcional* : indicara el tamaño en bytes del archivo.
  + **-cont**  *opcional* : indicara un archivo en el disco duro de la computadora que tendra el contenido del archivo.
  + **-stdin**  *opcional* : este parametro es mutuamente excluyete con el parametro *-cont* indica que el contenido del archivo a crear sera obtenido del standard input.

- **Ejemplos**
  Crea el archivo a.txt, si no existen las carpetas se crean, con tamaño de 15 bytes
  `touch -SIZE=15 -PatH=/home/user/docs/a.txt –r`
  Crea el archivo prometheus.service
  `touch -stdin -path=/etc/systemd/system/prometheus.service`
* * *
- **CAT** :
Este comando permitira mostrar el contenido del archivo, si el usuario que actualmente este logueado tiene acceso al permiso de lectura.
Tendra una serie de parametros los cuales son :

  + **-filen**  *obligatorio* : permitira admitir como argumentos una lista de n ficheros que hay que enlazar.

- **Ejemplos**
  Lee el archivo a.txt
  `Cat –file1=/home/user/docs/a.txt`
  Enlaza los archivos
  `Cat -file1="/home/a.txt" -file2="/home/b.txt" -file3="/home/c.txt"`
* * *
- **RM** :
Este comando permitira eliminar un archivo o carpeta y todo su contenido, si el usuario que actualmente esta logueado tiene acceso al permiso de escritura sobre el archivo y en el caso de carpetas eliminara todos los archivos y subcarpetas.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : sera la ruta del archivo o carpeta a eliminar.

- **Ejemplos**
  Elimina el archivo a.txt
  `rm -PatH=/home/user/docs/a.txt`
  Elimina la carpeta user y todo su contenido
  `rm -PatH=/home/user/docs/a.txt`
* * *
- **EDIT** :
Este comando permitira editar el contenido de un archivo para asignarle otro contenido.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : sera la ruta del archivo a editar.
  + **-cont**  *obligatorio* : indicara un archivo en el disco duro de la computadora que tendra el contenido del archivo.
  + **-stdin**  *opcional* : este parametro es mutuamente excluyete con el parametro *-cont* indica que el contenido del archivo a crear sera obtenido del standard input

- **Ejemplos**
  Modifica el archivo a.txt
  `Edit -PatH=/home/user/docs/a.txt -cont=/root/user/files/a.txt`
  `Edit -PatH=/home/user/docs/a.txt -stdin`
* * *
- **REN** :
Este comando permitira cambiar el nombre de un archivo o carpeta.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : sera la ruta del archivo o carpeta a editar.
  + **-name**  *obligatorio* : especifica el nombre nuevo del archivo.

- **Ejemplos**
  Cambia el nombre del archivo a.txt a b1.txt
  `ren -PatH=/home/user/docs/a.txt –name=b1.txt`
* * *
- **MKDIR** :
Este comando es similar a touch, pero no crea archivos sino carpetas.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : sera la ruta de la carpeta a crear.
  + **-p**  *opcional* : si se utiliza este parametro las carpetas que no existan en el path se crearan automaticamente.

- **Ejemplos**
  Crea la carpeta usac
  `Mkdir –P -path=/home/user/docs/usac`
* * *
- **CP** :
Este comando permitira realizar una copia del archivo o carpeta y todo su contenido hacia otro destino.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : sera la ruta del archivo o carpeta a copiar.
  + **-dest**  *obligatorio* : sera la ruta destino donde se copiara el contenido.

- **Ejemplos**
  Copia documents a imagenes
  `cp -Path="/home/user/documents" -dest="/home/images"`
* * *
- **MV** :
Este comando movera un archivo o carpeta y todo su contenido hacia otro destino.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : sera la ruta del archivo o carpeta a mover.
  + **-dest**  *obligatorio* : sera la ruta destino donde se movera el contenido.

- **Ejemplos**
  Copia documents a imagenes
  `mv -Path="/home/user/documents" -dest="/home/images"`
* * *
- **FIND** :
Este comando permitira realizar una busqueda por el nombre del archivo o carpeta.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : sera la ruta de la carpeta a buscar.
  + **-name**  *obligatorio* : indica el nombre de la carpeta o archivo que se desea buscar.

- **Ejemplos**
  Busca los archivos con extension *.js*
  `find -Path="/" -name=*.js`
* * *
- **PAUSE** :
Este comando permitira pausar la ejecucion del programa en un script cuando se utiliza el comando exec
- **Ejemplos**
  Busca los archivos con extension *.js*
  `pause`
* * *
- **EXEC** :
El programa podra ejecutar scripts con el comando exec.
Tendra una serie de parametros los cuales son :

  + **-path**  *obligatorio* : sera la ruta del script que se va a ejecutar.

- **Ejemplos**
  Ejecuta el script
  `exec -path=/home/Desktop/calificacion.sh`
* * *
- **REP** :
El programa podra ejecutar scripts con el comando exec.
Tendra una serie de parametros los cuales son :

  + **-name**  *obligatorio* : nombre del reporte a generar: **MBR, disk, inode, Journaling, block, bm_inode, bm_block, tree, sb, file, ls**.
  + **-path**  *obligatorio* : indica una carpeta y el nombre que tendra el reporte.
  + **-id**  *obligatorio* : indica el id de la particion que se utilizara.
  + **-ruta**  *opcional* : funcionara para el reporte *file* y *ls*, sera el nombre del archivo o carpeta.
  + **-root**  *opcional* : para uso exclusivo del reporte *tree*, indica el indice del inodo que se utilizara como raiz para el reporte.

- **Ejemplos**
  Reporte mbr
  `rep –id=561A -Path=/home/user/reports/reporte1.jpg -name=mbr`
* * *