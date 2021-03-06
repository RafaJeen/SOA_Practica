#include "ext.h"


void EXT_mostraInfoExt2(Ext ext){
    printf("\n-------- Fylesystem Information --------\n\n");
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

    time_t ll = (time_t)ext.s_mtime;
    char* lastMounted = ctime(&ll);
    printf("Ultim muntatge: %s\n", lastMounted);

    time_t yy = (time_t)ext.s_wtime;
    char* checkInterval = ctime(&yy);
    printf("Ultima escriptura: %s\n", checkInterval);
}

void deleteInode(int fd, Ext ext, int numInode){
    Inode inode;
    GroupDescriptor gd;

    int blockSize = 1024 << ext.s_log_block_size; //Calculem la mida d'un bloc

    lseek(fd, (ext.s_first_data_block+1)*blockSize, SEEK_SET); //Ens movem al bloc que conte el superblock

    read(fd, &gd, sizeof(GroupDescriptor)); //Llegeixo el group descriptor que conte el offset de la inode table

    int blockGroup = (numInode - 1) / ext.s_inodes_per_group; //Calculo l'inode relatiu per que sigui generic
    int relativeInode = (numInode - 1) % ext.s_inodes_per_group; //Calculo el grup en el que es troba

    int inodePosition = relativeInode*ext.s_inode_size; //Calculo el offset del inode respecte el comen??ament de la inode table
    int inodeTablePosition =  gd.bg_inode_table + blockGroup * ext.s_blocks_per_group; //Calculo el offset de la inode table

    lseek(fd, (inodeTablePosition*blockSize)+inodePosition, SEEK_SET); //Em despla??o a la posicio on es troba l'inode

    read(fd, &inode, sizeof(Inode));

    for(int i = 0; i < 15; i++){
        inode.i_block[i] = 0;
    }

    inode.i_size = 0;
    inode.i_dtime = time(0);
    inode.i_flags = 0x01;

    lseek(fd, (inodeTablePosition*blockSize)+inodePosition, SEEK_SET);
    write(fd, &inode, sizeof(Inode)); //Sobreescribim l'inode amb els camps modificats
}

int EXT_findFile(int fd, Ext ext, char* fileName, int numInode, int deleteFile) {
    int totalLen = 0;
    int actualLen = 0;
    int acabaLlegir = 0;
    int previousLen = 0;
    int bytes = -1;
    DirectoryEntry de;
    DirectoryEntry aux;
    char *name;
    Inode inode = EXT_getInode(fd, ext, numInode);
    int blockSize = 1024 << ext.s_log_block_size;

    for(int i = 0; totalLen < inode.i_size; i++){
        actualLen = 0;
        acabaLlegir = 0;
        lseek(fd, blockSize + blockSize*(inode.i_block[i]-1)+totalLen, SEEK_SET);
        while(!acabaLlegir){
            read(fd, &de, sizeof(DirectoryEntry));
            name = (char *) malloc (sizeof(char)*(de.name_len+1));
            read(fd, name, de.name_len);
            name[de.name_len] = '\0';
            actualLen = de.rec_len;
            totalLen = totalLen + actualLen;
            if(actualLen == 0 || totalLen > inode.i_size){
                acabaLlegir = 1;
            } else {
                if(strcmp(fileName, name) == 0 && de.file_type == 1){
                    if(deleteFile){
                        //Sumem al recLen del directoryEntry anterior el recLen del fitxer que eliminem
                        lseek(fd, blockSize + blockSize*(inode.i_block[i]-1)+totalLen-previousLen-actualLen, SEEK_SET);
                        read(fd, &aux, sizeof(DirectoryEntry));

                        aux.rec_len = aux.rec_len + de.rec_len;
                        lseek(fd, blockSize + blockSize*(inode.i_block[i]-1)+totalLen-previousLen-actualLen, SEEK_SET);
                        write(fd, &aux, sizeof(DirectoryEntry));

                        //canviem parametres de l'inode del fitxer que volem eliminar
                        deleteInode(fd, ext, de.inode);

                        //modifiquem el directory entry del fitxer que volem eliminar (un cop ja s'han modificat els camps de l'inode)
                        de.inode = 0;
                        lseek(fd, blockSize + blockSize*(inode.i_block[i]-1)+totalLen-actualLen, SEEK_SET);
                        write(fd, &de, sizeof(DirectoryEntry));

                        //modifiquem el super block per incrementar el contador d'inodes lliures
                        ext.s_free_inodes_count = ext.s_free_inodes_count + 1;
                        lseek(fd, 1024, SEEK_SET);
                        write(fd, &ext, sizeof(Ext));
                    }
                    Inode inode = EXT_getInode(fd, ext, de.inode);
                    bytes = inode.i_size;
                    acabaLlegir = 1;
                    totalLen = inode.i_size+5;
                } else if(de.file_type == 2) {
                    if(strcmp(name, ".") != 0 && strcmp(name, "..") != 0 && strcmp(name, "lost+found") != 0){
                        bytes = EXT_findFile(fd, ext, fileName, de.inode, deleteFile);
                        if(bytes>=0){
                            acabaLlegir = 1;
                            totalLen = inode.i_size+5;
                        }
                    }
                }
                lseek(fd, blockSize + blockSize*(inode.i_block[i]-1)+totalLen, SEEK_SET);
                free(name);
            }
            previousLen = actualLen;
        }
    }
    return bytes;
}

Inode EXT_getInode(int fd, Ext ext, int numInode) {
    Inode inode;
    GroupDescriptor gd;

    int blockSize = 1024 << ext.s_log_block_size; //Calculem la mida d'un bloc

    lseek(fd, (ext.s_first_data_block+1)*blockSize, SEEK_SET); //Ens movem al bloc que conte el superblock

    read(fd, &gd, sizeof(GroupDescriptor)); //Llegeixo el group descriptor que conte el offset de la inode table

    int blockGroup = (numInode - 1) / ext.s_inodes_per_group; //Calculo l'inode relatiu per que sigui generic
    int relativeInode = (numInode - 1) % ext.s_inodes_per_group; //Calculo el grup en el que es troba

    int inodePosition = relativeInode*ext.s_inode_size; //Calculo el offset del inode respecte el comen??ament de la inode table
    int inodeTablePosition =  gd.bg_inode_table + blockGroup * ext.s_blocks_per_group; //Calculo el offset de la inode table

    lseek(fd, (inodeTablePosition*blockSize)+inodePosition, SEEK_SET); //Em despla??o a la posicio on es troba l'inode
    read(fd, &inode, sizeof(Inode)); //Llegeixo l'inode

    return inode;
}
