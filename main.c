#include <stdio.h>
#include <stdlib.h>
#include "ext.h"
#include "fat16.h"

int getKey(char *command) {
    if(strcmp(command, "/info") == 0) return 1;
    if(strcmp(command, "/find") == 0) return 2;
    if(strcmp(command, "/delete") == 0) return 3;
    return 0;
}

int main (int argc, char *argv[]){

    int fd = open(argv[2], O_RDWR);

    if(fd < 0){
        printf("Error. Volum inexistent.\n");
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
      read(fd, &ext, sizeof(Ext)); //Llegim el superblock
      switch(getKey(argv[1])) {
          case 1: //info
              if(argc != 3){
                  printf("El nombre de parametres es incorrecte.\n");
                  return 0;
              } else {
                  EXT_mostraInfoExt2(ext);
              }
              break;
          case 2:; //find
              if(argc != 4) {
                  printf("El nombre de parametres es incorrecte.\n");
                  return 0;
              } else {
                  int bytes = EXT_findFile(fd, ext, argv[3], 2, 0);
                  if(bytes < 0) {
                      printf("Error. Fitxer no trobat.\n");
                  } else {
                      printf("Fitxer trobat. Ocupa %d bytes.\n", bytes);
                  }
              }
              break;
          case 3: //delete
              if(argc != 4) {
                  printf("El nobre de parametres es incorrecte.\n");
              } else {
                  //eliminar el fichero
                  int bytes = EXT_findFile(fd, ext, argv[3], 2, 1);
                  if(bytes >= 0){
                      printf("El fitxer %s ha estat eliminat.\n", argv[3]);
                  } else {
                      printf("Error. Fitxer no trobat.\n");
                  }
              }
              break;
          default:
              printf("La comanda %s es incorrecte.\n", argv[1]);
              break;
      }
      close(fd);
    } else {
      //calculamos para comprobar que es un fichero fat16
      if(FAT_isFat16(fd)){
        Fat fat;
        FAT_llegeixInfoFat(&fat, fd);
        switch(getKey(argv[1])) {
          case 1: //info
              if(argc != 3) {
                  printf("El nombre de parametres es incorrecte.\n");
                  return 0;
              } else {
                  FAT_mostraInfoFat16(fat);
              }
              break;
          case 2: //find
              if(argc != 4) {
                  printf("El nombre de parametres es incorrecte.\n");
                  return 0;
              } else {
                  char nomFitxer[30];
                  char extensio[4];
                  FAT_separaExtensio(argv[3], nomFitxer, extensio);
                  if(strlen(nomFitxer) > 6){
                      nomFitxer[6] = '\0';
                  }

                  int fileBytes = FAT_findFileInFat(fd, fat, nomFitxer, extensio, 2, 0);
                  if(fileBytes >= 0) {
                      printf("Fitxer trobat. Ocupa %d bytes.\n", fileBytes);
                  } else {
                      printf("Error. Fitxer no trobat.\n");
                  }
              }
              break;
          case 3: //delete
              if(argc != 4) {
                 printf("El nobre de parametres es incorrecte.\n");
              } else {
                  //eliminar el fichero
                  char nomFitxer[30];
                  char extensio[4];
                  FAT_separaExtensio(argv[3], nomFitxer, extensio);
                  if(strlen(nomFitxer) > 6){
                      nomFitxer[6] = '\0';
                  }
                  int bytes = FAT_findFileInFat(fd, fat, nomFitxer, extensio, 2, 1);
                  if(bytes >= 0){
                      printf("El fitxer %s ha estat eliminat.\n", argv[3]);
                  } else {
                      printf("Error. Fitxer no trobat.\n");
                  }
              }
              break;
          default:
              printf("La comanda %s es incorrecte.\n", argv[1]);
              break;
        }
        close(fd);
      } else {
        printf("Error. Volum no formatat en FAT16 ni EXT2.\n");
      }
    }
    return 0;
}
