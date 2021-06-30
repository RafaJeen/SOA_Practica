# SOA_Practica
## MEMORIA

### EXPLICACIÓ PER PODER COMPILAR LA PRÀCTICA

Aquesta pràctica ha estat realitzada amb el llenguatge de programació C. Consta de diferents
moduls per lo que te un makefile per poder generar un executable que serà executat amb la 
comanda `./nomExecutable` que en aquest cas te el nom de _shooter_. Per poder compilar la 
pràctica s'han de realitzar els següents pasos: 
* Anar a la carpeta [`/`](main) al terminal que es on es troben tots els arxius necessaris.
* Executar la comanda `make clean`.
* Seguidament, s'ha d'executar la comanda `make`.
* Un cop generat l'executable, ja es pot executar el programa realitzant una comanda com la 
següent:

`./shooter <nom del disc> <nom de la operacio> <nom del fitxer>`

sent l'ultim paràmetre opcional depenent de la funcionalitat que es vulgui executar.

### EXPLICACIÓ DELS SISTEMES DE FITXERS

* #### EXT2

Ext 2 és un sistema de fitxers que treballa amb inodes. Aquests inodes son els que guarden
la informació dels diferents arxius com la seva ruta, mida, etc. Aquest sistema de fitxers està
format per diferents N block grups (diferents grups que contenen la informació del volum). Cadascún 
d'aquests block grups internament conté diferents camps, alguns genèrics (com el superblock, 
group descriptor) on es pot trobar informació rellevant sobre el volum (on trobar els diferents blocs que hi han dins 
d'aquest block grup, diferents mides com la d'un inode, un bloc) que de cara a voler consultar informació
del diferents arxius es de gran ajuda. Per altre banda, te els grups interns de cada block grup on
cadascún conté diferent informació sobre els difernets arxius. 

![GitHub Logo](https://lh3.googleusercontent.com/proxy/6pYSYTb1UFweN0-ju2AV_VpUU8PjuVObxWz5J1tK1xDL9VCGr93HsQrFf94Uk6XfguxGqdLKLyXMLyVyT6ufhs0)

En aquesta imatge podem observar com estan estructurats els diferents blocs. 

En el bloc de _inode table_ es on es troben tots els inodes que conté el block group N.
Cadascún d'aquests inodes conté un array anomenat `i_blocks[15]` el qual conté diferents 
punters cap a diferents regions del _data block_ on es poden trobar els diferents __directory entries__.

En definitiva, cadascuna de les caselles de l'array de `i_blocks` conté diferents direcory entries 
on cadscún d'aquests conté diferent informació rellevant sobre l'arxiu.

* #### FAT16

Aquest sistema de fitxers treballa amb diferents blocs respecte l'anterior. En aquest cas, 
FAT16 conté els blocs _Boot Sector_, _FAT Data Structure_, _FAT Directory Structure_ i _Data Area_.

![GitHub Logo](http://4.bp.blogspot.com/-Br6_38Btdcc/T2H91dZzfAI/AAAAAAAAAOY/SeOR4oceRDQ/s1600/fat+16a.jpg)

En aquest sistema de fitxers, les diferents particions estàn formades per _clusters_ de mateixa mida,
que es la manera en la que un arxiu queda representat, com una cadena sequencial d'aquests.
Cada _cluster_ està enllaçat mitjançant un punter al següent de manera que queden lligats
(com una llista enllaçada).

Del primer bloc, és important coneixer que conté informació bàsica, punters cap a les següents seccions
cosa que es de gran ajuda si el que es vol es trobar diferents arxius del volum. El _FAT Data Structure_
conté dues copies de la taula __FAT__ que es la que indica quins són els clusters que es troben ocupatspels diferents arxius.
 Aquesta taula conté una llista d'entrades on cadascuna d'aquestes conté informacio sobre un cluster 
(la direcció del següent cluster, un indicador de si està lliure, etc.).
El _FAT Directory Structure_ es l'index principal de carpetes i arxius i per últim, el _Data Area_ conté
tot el contingut d'arxius i carpetes.

### EXPLICACIÓ DE LA PRÀCTICA

__1.__ __Requeriments__

Per aquesta pràctica el que s'havia de realitzar eren diferents funcionalitats tractant els 
sistemes de fitxers FAT16 i EXT2. Primer de tot, s'havia d'accedir al `Boot Sector` o al `Super Block`
depenent del tipus de disc i llegir la informació rellevant que es trobava sobre el volum. A més, també hi ha la part de buscar diferents arxius dins 
del volum (primerament nomes en el _Root_, i més endevant una busca en profunditat ono aquest arxiu es buscava
en tots els directoris). Finalment, també s'ha d'implementar la funcionalitat que et permet 
eliminar un dels arxius del volum que t'introdueixi l'usuari en cas de que aquest existeixi.
En cas contrari, es mostra un missatge d'error per pantalla informant de que no es un nom d'un arxiu 
existent.

__2.__ __Disseny__

Com en aquesta pràctica s'havia de treballar amb diferents sistemes de fitxers, he decidit dissenyar
la pràctica en diferents mòduls per tal de que totes les funcionalitats quedesin lo més organitzades posible.

![GitHub Logo](assets/moduls.png)

He creat un modul per cadascún dels sitemes de fitxers que contenen les diferents funcionalitats 
de cadascun d'aquests per separat. Dins de cadascun dels mòduls, per cadascuna de les funcionalitats 
he implementat una funció diferent per tal de que el codi em quedes lo més estructurat posible
segons els diferents casos en els que es podem trobar durant l'execució del programa.


