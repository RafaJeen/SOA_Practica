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

__3.__ __Estructures de dades__

```c
typedef struct {
  uint32_t s_inodes_count;
  uint32_t s_blocks_count;
  uint32_t s_r_blocks_count;
  uint32_t s_free_blocks_count;
  uint32_t s_free_inodes_count;
  uint32_t s_first_data_block;
  uint32_t s_log_block_size;
  int32_t s_log_frag_size;
  uint32_t s_blocks_per_group;
  uint32_t s_frags_per_group;
  uint32_t s_inodes_per_group;
  uint32_t s_mtime;
  uint32_t s_wtime;
  uint16_t s_mnt_count;
  uint16_t s_max_mnt_count;
  uint16_t s_magic;
  uint16_t s_state;
  uint16_t s_errors;
  uint16_t s_minor_rev_level;
  uint32_t s_lastcheck;
  uint32_t s_checkinterval;
  uint32_t s_creator_os;
  uint32_t s_rev_level;
  uint16_t s_def_resuid;
  uint16_t s_def_resgid;
  uint32_t s_first_ino;
  uint16_t s_inode_size;
  uint16_t s_block_group_nr;
  uint32_t s_feature_compat;
  uint32_t s_feature_incompat;
  uint32_t s_feature_ro_compat;
  uint32_t s_uuid[4];
  char s_volume_name[16];
  //uint32_t s_last_mounted[16];
  unsigned char s_last_mounted[64];
  uint32_t s_algo_bitmap;
}Ext;
```

Aquesta estructura l'he utilitzat per poder llegir el _SuperBlock_ del volum __EXT2__ de manera 
senzilla i directa.

```c
typedef struct{
    uint32_t bg_block_bitmap;
    uint32_t bg_inode_bitmap;
    uint32_t bg_inode_table;
    uint16_t bg_free_blocks_count;
    uint16_t bg_free_inodes_count;
    uint16_t bg_used_dirs_count;
    uint16_t bg_pad;
    unsigned char bg_reserved[12];
}GroupDescriptor;
```

Aquesta estructura ha estat utilitzada per poder llegir el bloc _GroupDescriptor_ de cop del 
volum __EXT2__ per ràpidament obtenir la informació necessària per poder trobar la taula d'inodes.

```c
typedef struct {
    uint16_t i_mode;        // Mode (type of file and permissions)
    uint16_t i_uid;         // Owner's user ID
    uint32_t i_size;        // File size (least significant 32 bits) <--
    uint32_t i_atime;       // Last access time
    uint32_t i_ctime;       // Creation time
    uint32_t i_mtime;       // Last modification time
    uint32_t i_dtime;       // Deletion time
    uint16_t i_gid;         // Owner's group ID
    uint16_t i_links_count; // Reference counter (number of hard links)
    uint32_t i_blocks;      // Number of 512-byte blocks reserved for this inode
    uint32_t i_flags;       // Flags
    uint32_t i_osd1;        // OS-dependant value
    uint32_t i_block[15];   //Blocs amb informacio <--
    uint32_t i_generation;  // File version (used for NFS)
    uint32_t i_file_acl;    // Block number for extended attributes
    uint32_t i_dir_acl;     // File size (most significant 32 bits)
    uint32_t i_faddr;       // Location of file fragment (deprecated)
    unsigned char i_osd2[12];
} Inode;
```

Aquesta estructura l'he utilitzat per poder llegir un inode de manera directa i d'aquesta forma
poder tenir acces al _size_ d'aquest inode i l'array de _i_blocks_ per poder començar a llegir les 
_directory entries_ de l'arxiu.

```c
typedef struct{
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
}DirectoryEntry;
```

Aquesta estructura l'he utilitzat per poder llegir els diferents directory entries de cop 
i d'aquesta manera poder anar a buscar l'inode d'aquest arxiu de manera directa o consultar el rec_len tambe de manera directa depenent de la situació.

```c
typedef struct {
  uint8_t BS_jmpBoot[3];
  unsigned char BS_OEMName[8];
  unsigned short BPB_BytsPerSec;
  uint8_t BPB_SecPerClus;
  short BPB_RsvdSecCnt;
  uint8_t BPB_NumFATs;
  unsigned short BPB_RootEnCnt;
  unsigned short BPB_TotSec16;
  uint8_t BPB_Media;
  short BPB_FATSz16;
  short BPB_SecPerTrk;
  short BPB_NumHeads;
  unsigned int BPB_HiddSec;
  unsigned int BPB_TotSec32;
  uint8_t BS_DrvNum;
  uint8_t BS_Reserved1;
  uint8_t BS_BootSig;
  unsigned int BS_VolID;
  char BS_VolLab[11];
}Fat;
```

Aquesta estructura ha estat utilitzada per poder realitzar la lectura del _Root Sector_ del volum __FAT16__
de manera molt més còmode i directa.

```c
typedef struct {
    char long_name[8];
    char extension[3];
    uint8_t fileAttr;
    uint8_t reserved[10];
    uint16_t tChange;
    uint16_t dChange;
    uint16_t firstCluster;
    uint32_t fSize;
}DirectoryEntryFat;
```

Estructura utilitzada per poder realitzar la lectura dels _Directory Entries_ de __FAT16__
de manera còmode i senzilla per tal de poder accedir a diferents camps d'aquesta estructura amb facilitat.

__4.__ __Proves realitzades__

El progrès de funcionalitats acabades durant la realització d'aquest projecte ha estat molt 
incremental ja que primerament m'he centrat molt en entendre be com funcionaven les diferents estructures de fitxers 
per lo que vaig començar llegint tota la documentació i realitzant proves constantment respecte
els nous coneixements que anava assolint (bàsicament, em centrava molt en anar printant diferents apartats del volum 
per comprovar si realment havia entès el que acabava de llegir). D'aquesta manera, realitzant tot aquest conjunt de proves 
he anat aconseguint entendre correctament el funcionament d'aquests sistemes de fitxers.
Un altre mètode de proves que he utilitzat, apart de printar tota mena d'informació per pantalla, ha estat per la ultima fase 
(en la qual s'haviia d'eliminar un arxiu), ha estat realitzar un `mount` sobre una copia del volum original 
en una màquina virtual despres d'haver executat el programa sobre aquest volum per comprovar que e fitxer s'estigues eliminant correctament.

__5.__ __Problemes observats__

Considero que el major problema que he tingut en aquest projecte ha estat lo que m'ha costat 
acabar d'entendre be quin era el funcionament dels diferents sistemes de fitxers, ja que 
això ha causat que moltes de les funcionalitats que volia implementar no funcionesin i era per
falta de coneixements sobre els volums que no funcionava correctament. El mètode de proves utilitzat ha estat molt bàsic,
buscar molta informació sobre el funciionament dels volums per acabar d'entendrel's be i tambe sobretot
printar moltes de les dades consultades per pantalla per poder comprovar si el que havia realitzat ja estava be 
i saber si anava ben encaminat. Realitzant tot aquest conjunt de proves he anant aprenent lo suficient com per poder acabar
la pràctica, lo qual ha estat el progrès d'aprenentatge i de solució d'errors principal durant el desenvolupament
d'aquest projecte.

 __6.__ __Estimacio temporal__
 
 * __Fase 1:__ 3h.
 * __Fase 2:__ 8h.
 * __Fase 3:__ 5h.
 * __Fase 4:__ 2h.
 
 Considero que les fases 1 i 2 han estat les que mes m'han costat de realitzar ja que vaig haver
 de dedicar molt de temps en informarme sobre els diferents sistemes de fitxers i al no tenir molt
 clars diferents aspectes rellevants al principi, em sortien una quantitat considerable d'errors que em van ser bastant 
 costosos de solucionar per falta de coneixements.
 
 ### VALORACIO DE GIT
 
 Penso que ha estat de gran ajuda utilitzar aquesta eina per poder tenir un correcte 
 control sobre les diferents versions del projecte. Encara que penso que hagues estat encara molt més útil
 si aquesta pràctica hagués estat en parelles ja que considero que el gran potencial d'utilitzar aquesta eina
 es quan es treballa desde diferents dispositius. Encara que igualment, com ja he comentat anteriorment,
 m'ha ajudat molt a poder tenir tot el projecte molt més estructurat.
 
 ![GitHub Logo](assets/moduls.png)
 
 ### CONCLUSIONS GENERALS
 
 Aquest projecte m'ha estat bastant costòs sobretot pel fet d'haver-nos de "buscar la vida"
 per informar-nos sobre tot lo que contenen aquests sistemes de fitxers (__FAT16__ i __EXT2__ )
 ja que això ha ocasionat que en molts casos consideres que ja tingues els suficients coneixements com per poder implementar 
 diferents funcionalitats pero al no ser aixi, m'he anat trobant amb diferents errors ocasionants per aquesta falta de coneixements
 sobre diferents aspectes importants dels volums que han estat molt costosos de trobar. Treient això, 
 considero que es un projecte que m'ha ajudat molt a buscar-me la vida a l'hora d'haver de buscar informació sobre algo 
 que de primeres es totalment desconegut. Finalment, aquest projecte m'ha semblat molt interesant ja que
 m'ha ajudat a entendre molt millor com funciona tota la gestió de fitxers que sempre m'ha semblat molt interesant.
 