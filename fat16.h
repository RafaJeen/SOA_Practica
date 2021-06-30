#ifndef _FAT16_H_
#define _FAT16_H_

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


#define _FAT16_H_

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


int FAT_isFat16(int fd);
char *FAT_netejaCadena(char*cadena);
void FAT_llegeixInfoFat(Fat *fat, int fd);
void FAT_mostraInfoFat16(Fat fat);
void FAT_separaExtensio(char* nomTotal, char* nomFitxer, char* extensio);
int FAT_findFileInFat(int fd, Fat fat, char* nomFitxer, char* extensio, int blockNum, int deleteFile);



#endif

