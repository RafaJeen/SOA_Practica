#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


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
        printf("Aquest error noexiteix.\n");
        return 0;
    } else {
        printf("LO HE ABIERTO!!!!\n");
    }


    lseek(fd, 1024, SEEK_SET);

    int num;

    read(fd, &num, 4);

    printf("%d\n", num);

}