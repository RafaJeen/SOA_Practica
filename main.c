#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>

typedef struct {
  uint32_t s_inodes_count; //Alomejor son todos int pk 4bytes*8 = 32bits
  uint32_t s_blocks_count;
  uint32_t s_r_blocks_count;
  uint32_t s_free_blocks_count;
  uint32_t s_free_inodes_count;
  uint32_t s_first_data_block;
  uint32_t s_log_block_size; //Nose se si uint16 (especifica solo numeros positivos)
  int32_t s_log_frag_size; //Especificaque pilla valores negativos y positivos
  uint32_t s_blocks_per_group;
  uint32_t s_frags_per_group;
  uint32_t s_inodes_per_group;
  uint32_t s_mtime; //Guarda  fecha --> int64_t (otra prueba)----
  uint32_t s_wtime; //Lo mismo q arriba --time_t----
  uint16_t s_mnt_count; //Guarda un tiempo (no se si int)
  uint16_t s_max_mnt_count; //Puede q int
  uint16_t s_magic;
  uint16_t s_state; //Si falla mirar tabla
  uint16_t s_errors; //Lo mismo que arriba
  uint16_t s_minor_rev_level; //Alomejor int
  uint32_t s_lastcheck; //Lo mismo q arriba --time_t ------
  uint32_t s_checkinterval; //Lo mismo q arriba --time_t
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
  uint32_t s_uuid[4]; //128bit unico k esta bien creojeje
  unsigned char s_volume_name[16];
  //uint32_t s_last_mounted[16];
  char s_last_mounted[64];
  uint32_t s_algo_bitmap; //32bit creo k d los pocos k bnn
  uint8_t s_prealloc_blocks; //Creo que asi tiene que ser :)
  uint8_t s_prealloc_dir_blocks; //Probar
  uint16_t s_journal_uuid; //Probar
  int32_t s_journal_inum; //Probar
  int32_t s_journal_dev;
  int32_t s_last_orphan;
  int32_t s_hash_seed[4]; //Probar
  int8_t s_def_hash_version; //Probar
  int32_t s_default_mount_options;
  int32_t s_first_meta_bg; //32bit
}Ext;



void mostraInfoExt2(Ext ext){

    printf("-------- Fylesystem Information --------\n\n");
    printf("Filesystem: EXT2\n\n");

    printf("INFO INODE\n");
    printf("Mida Inode: %d\n", ext.s_inode_size);
    printf("Num Inodes: %d\n", ext.s_inodes_count);
    printf("Primer Inode: %d\n", ext.s_first_ino);
    printf("Inodes Grup: %d\n", ext.s_inodes_per_group);
    printf("Inodes Lliures: %d\n", ext.s_free_inodes_count);

    printf("\nINFO BLOC\n");
    int block_size = 1024 << ext.s_log_block_size;
    printf("Mida Bloc: %d\n", block_size);
    printf("Blocs Reservats: %d\n", ext.s_r_blocks_count);
    printf("Blocs Lliures: %d\n", ext.s_free_blocks_count);
    printf("Total Blocs: %d\n", ext.s_blocks_count);
    printf("Primer Bloc: %d\n", ext.s_first_data_block);
    printf("Blocs group: %d\n", ext.s_blocks_per_group);
    printf("Frags group: %d\n", ext.s_frags_per_group);

    printf("\nINFO VOLUM\n");
    printf("Nom volum: %s\n", ext.s_volume_name);

    time_t kk = (time_t)ext.s_lastcheck;
    char* lastCheck = ctime(&kk);
    printf("Ultima comprov: %s\n", lastCheck);

    kk = (time_t)ext.s_last_mounted;
    char* lastMounted = ctime(&kk);
    printf("Ultim muntatge: %s\n", lastMounted);

    kk = (time_t)ext.s_checkinterval;
    char* checkInterval = ctime(&kk);
    printf("Ultima escriptura: %s\n", checkInterval);
}

int isFat16(int fd){
  lseek(fd, 0, SEEK_SET);

  short rootDirSectors, BPB_RootEntCnt, BPB_BytsPerSec;
  //short BPB_RootEntCnt;

  lseek(fd, 17, SEEK_CUR);
  read(fd, &BPB_RootEntCnt, 2);
  lseek(fd, 11, SEEK_SET);
  read(fd, &BPB_BytsPerSec, 2);

  rootDirSectors = ((BPB_RootEntCnt * 32) + (BPB_BytsPerSec - 1)) / BPB_BytsPerSec;

  printf("%d\n", rootDirSectors);

  lseek(fd, 22, SEEK_SET);

  short BPB_FATSz16;
  read(fd, &BPB_FATSz16, 2);

  short FATSz;

  if(BPB_FATSz16 != 0){
    FATSz = BPB_FATSz16;
  }

  printf("%d\n", FATSz);

  lseek(fd, 19, SEEK_SET);

  short BPB_TotSec16;

  read(fd, &BPB_TotSec16, 2);

  printf("%d\n", BPB_TotSec16);

  short TotSec;

  if(BPB_TotSec16 != 0){
    TotSec = BPB_TotSec16;
  }

  short BPB_RsvdSecCnt;
  lseek(fd, 14, SEEK_SET);
  read(fd, &BPB_RsvdSecCnt, 2);

  short BPB_NumFats;
  lseek(fd, 16, SEEK_SET);
  read(fd, &BPB_NumFats, SEEK_SET);

  short DataSec = TotSec - (BPB_RsvdSecCnt + (BPB_NumFats * FATSz) + rootDirSectors);

  printf("%d\n", DataSec);

  short BPB_SetPerClus;
  lseek(fd, 13, SEEK_SET);
  read(fd, &BPB_SetPerClus, 1);

  //short setPerClus = BPB_SetPerClus - '0';

  printf("%d\n", BPB_SetPerClus);

  short countOfClousters = DataSec / BPB_SetPerClus;

  printf("%d\n", countOfClousters);

  if(countOfClousters >= 4085 && countOfClousters < 65525){
    return 1;
  } else {
    return 0;
  }
}


int main (int argc, char *argv[]){

    if(argc != 3){
        printf("El nombre de parametres es incorrecte.\n");
        return 0;
    }

    if(strcmp(argv[1], "/intro") != 0){
        printf("La comanda %s es incorrecte\n", argv[1]);
        return 0;
    }

    int fd = open(argv[2], O_RDONLY);

    if(fd < 0){
        printf("Aquest fitxer no existeix.\n");
        return 0;
    }

    lseek(fd, 1024, SEEK_CUR); //Situamos cursor en la parte donde empieza la info que nos inetresa

    lseek(fd, 56, SEEK_CUR);

    int h = 0;

    read(fd, &h, 2);


    if(h == 0xEF53){
      //Es un fitxer Ext2
      lseek(fd, 1024, SEEK_SET); //Tornem a situar el cursor on comença la info interesant
      Ext ext;
      read(fd, &ext, sizeof(Ext));

      mostraInfoExt2(ext);
      close(fd);
    } else {
      //calculamos para comprobar que es un fichero fat16
      if(isFat16(fd)){
          printf("lelelel\n");
      }
    }









}
