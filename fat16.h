/*#ifndef _FAT16_H_
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
  // unsigned int BS_VolID;
  char BS_VolLab[11];
}Fat;


int isFat16(int fd);
char *netejaCadena(char*cadena);
void mostraInfoFat16(Fat fat);


#endif*/
