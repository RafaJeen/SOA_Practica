#include "fat16.h"


int FAT_isFat16(int fd){
  int trobat = 0;

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
    trobat = 1;
  }
  return trobat;
}

void FAT_llegeixInfoFat(Fat *fat, int fd) {
    lseek(fd, 0, SEEK_SET);
    read(fd, &fat->BS_jmpBoot, 3);
    read(fd, fat->BS_OEMName, 8);
    read(fd, &fat->BPB_BytsPerSec, 2);
    read(fd, &fat->BPB_SecPerClus, 1);
    read(fd, &fat->BPB_RsvdSecCnt, 2);
    read(fd, &fat->BPB_NumFATs, 1);
    read(fd, &fat->BPB_RootEnCnt, 2);
    read(fd, &fat->BPB_TotSec16, 2);
    read(fd, &fat->BPB_Media, 1);
    read(fd, &fat->BPB_FATSz16, 2);
    read(fd, &fat->BPB_SecPerTrk, 2);
    read(fd, &fat->BPB_NumHeads, 2);
    read(fd, &fat->BPB_HiddSec, 4);
    read(fd, &fat->BPB_TotSec32, 4);
    read(fd, &fat->BS_DrvNum, 1);
    read(fd, &fat->BS_Reserved1, 1);
    read(fd, &fat->BS_BootSig, 1);
    read(fd, &fat->BS_VolID, 4);
    read(fd, fat->BS_VolLab, 11);
}


char *FAT_netejaCadena(char*cadena){
   char *label = malloc(10);
   int i;

   for(i = 0; cadena[i] != ' '; i++){
      label[i] = cadena[i];
   }

   label[i] = '\0';

   return label;

}

void FAT_mostraInfoFat16(Fat fat){
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
  char *label = FAT_netejaCadena(fat.BS_VolLab);
  printf("Label: %s\n", label);
}

void  FAT_separaExtensio(char* nomTotal, char* nomFitxer, char* extensio) {
    int k = 0;
    extensio[0] = '\0';
    int trobat = 0;

    for(int i = strlen(nomTotal)-1; i >= 0; i--) {
        if(nomTotal[i] == '.'){
            trobat = 1;
            for(k = i; k < strlen(nomTotal); k++){
                extensio[k-(i+1)] = nomTotal[k];
            }
            nomTotal[i] = '\0';
            strcpy(nomFitxer, nomTotal);
            extensio[k-(i+1)] = '\0';
            break;
        }
    }
    if(!trobat) {
      strcpy(nomFitxer, nomTotal);
   }
}

void FAT_clearString(char *string, int limit) {
    int j = 0;
    int i = 0;

    for(i = 0; i <= limit; i++){
        if(string[i] != ' '){
            if(string[i] >= 'A' && string[i] <= 'Z'){
                string[j] = string[i] - 'A' + 'a';
            } else {
                string[j] = string[i];
            }
            j++;
        } else {
            //string[i] = '\0';
            string[j] = '\0';
            break;
        }
    }
    string[j] = '\0';
}

int FAT_findFileInFat(int fd, Fat fat, char* nomFitxer, char* extensio, int blockNum){

    uint32_t rootDirectoryRegion = fat.BPB_RootEnCnt * 32;

    uint32_t RootDirectoryRegionStart = (fat.BPB_RsvdSecCnt+(fat.BPB_NumFATs*fat.BPB_FATSz16))* fat.BPB_BytsPerSec ;

    uint32_t DataRegionStart = ((blockNum-2) * fat.BPB_SecPerClus * fat.BPB_BytsPerSec) + RootDirectoryRegionStart ;

    if(blockNum!=2){
        DataRegionStart = DataRegionStart + rootDirectoryRegion;
    }

    lseek(fd, DataRegionStart, SEEK_SET);

    int surt = 0;
    int bytes = -1;
    DirectoryEntryFat de;

    for(int k=0; !surt; k++) {
        read(fd, &de, sizeof(DirectoryEntryFat));
        if(de.long_name[0] == 0x00){
            surt = 1;
        } else {
            FAT_clearString(de.long_name, 8);
            FAT_clearString(de.extension, 3);

            if(strlen(de.long_name) > 6){
                /*if(de.long_name[6] == '~' && de.long_name[7] == '1'){
                    de.long_name[6] = '\0';
                } else {*/
                    de.long_name[6] = '\0';
                //}
            }

            //Comprovem que no tingui la extensio directament enganxada
            if(strlen(de.extension) > 0){
                char aux[4];
                int k = strlen(de.long_name) - (strlen(de.extension));
                int j = 0;
                for(int i = k; i < strlen(de.long_name); i++){
                    aux[j] = de.long_name[i];
                    j++;
                }
                aux[j] = '\0';
                if(strcmp(aux, de.extension) == 0){
                    de.long_name[k] = '\0';
                }
            }

            for(int i=0; de.long_name[i]!='\0' && i<8 ;i++){
                if(de.long_name[i]==' '){
                    de.long_name[i]='\0';
                    break;
                }
            }

            if(de.fileAttr == 16 && strcmp(de.long_name, ".") != 0 && strcmp(de.long_name, "..") != 0){
                bytes = FAT_findFileInFat(fd, fat, nomFitxer, extensio, de.firstCluster);
                if(bytes>=0){
                    surt = 1;
                }
                lseek(fd, DataRegionStart+k*(sizeof(DirectoryEntryFat)), SEEK_SET);
            } else
                if(strcmp(nomFitxer, de.long_name) == 0){
                    if(strlen(de.extension) > 0){
                        if(strcmp(de.extension, extensio) == 0){
                            bytes = de.fSize;
                            surt = 1;
                        }
                    } else {
                        if(strlen(extensio) == 0){
                            bytes = de.fSize;
                            surt = 1;
                        }
                    }
                }
            }
        }
    return bytes;
}
