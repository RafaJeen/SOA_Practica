#include "ext.h"


void mostraInfoExt2(Ext ext){
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
