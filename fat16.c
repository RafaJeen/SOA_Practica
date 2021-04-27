#include "fat16.h"


int isFat16(int fd){
  lseek(fd, 0, SEEK_SET);

  short rootDirSectors, BPB_RootEntCnt, BPB_BytsPerSec;

  lseek(fd, 17, SEEK_CUR);
  read(fd, &BPB_RootEntCnt, 2);
  lseek(fd, 11, SEEK_SET);
  read(fd, &BPB_BytsPerSec, 2);

  rootDirSectors = ((BPB_RootEntCnt * 32) + (BPB_BytsPerSec - 1)) / BPB_BytsPerSec;

  lseek(fd, 22, SEEK_SET);

  short BPB_FATSz16;
  read(fd, &BPB_FATSz16, 2);

  short FATSz;

  if(BPB_FATSz16 != 0){
    FATSz = BPB_FATSz16;
  }

  lseek(fd, 19, SEEK_SET);

  short BPB_TotSec16;

  read(fd, &BPB_TotSec16, 2);

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

  short BPB_SetPerClus;
  lseek(fd, 13, SEEK_SET);
  read(fd, &BPB_SetPerClus, 1);

  short countOfClousters = DataSec / BPB_SetPerClus;

  if(countOfClousters >= 4085 && countOfClousters < 65525){
    return 1;
  } else {
    return 0;
  }
}


char *netejaCadena(char*cadena){
   char *label = malloc(10);
   int i;

   for(i = 0; cadena[i] != ' '; i++){
      label[i] = cadena[i];
   }

   label[i] = '\0';

   return label;

}

void mostraInfoFat16(Fat fat){
  printf("\n-------- Fylesystem Information --------\n\n");
  printf("Filesystem: FAT16\n");
  printf("System Name: %s\n", fat.BS_OEMName);
  printf("Mida del sector: %d\n", fat.BPB_BytsPerSec);

  int sec = (int) fat.BPB_SecPerClus;
  printf("Sectors Per Cluster: %d\n", sec);

  printf("Sectors reservats: %d\n", fat.BPB_RsvdSecCnt);

  int fats = (int) fat.BPB_NumFATs;
  printf("Numero de FATs: %d\n", fats);
  printf("MaxRootEntries: %d\n", fat.BPB_RootEnCnt);
  printf("Sectors per FAT: %d\n", fat.BPB_TotSec16);
  char *label = netejaCadena(fat.BS_VolLab);
  printf("Label: %s\n", label);
}
