#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>


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

    uint16_t h;

    read(fd, &h, 2);

    printf("%x\n", h);

    if(h == 0xEF53){
      //Es un fitxer Ext2
      //holahola
    }




}
