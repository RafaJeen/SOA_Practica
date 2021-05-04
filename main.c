#include <stdio.h>
#include <stdlib.h>
#include "ext.h"
#include "fat16.h"

int main (int argc, char *argv[]){

    if(argc != 3){
        printf("El nombre de parametres es incorrecte.\n");
        return 0;
    }

    if(strcmp(argv[1], "/info") != 0){
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
        Fat fat;
        lseek(fd, 0, SEEK_SET);
        read(fd, &fat, sizeof(fat));
        //printf("");
        mostraInfoFat16(fat);
      } else {
        printf("L'arxiu que has introduit no es un fitxer valid.\n");
      }
      close(fd);
    }
}
