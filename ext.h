#ifndef _EXT_H_
#define _EXT_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>

typedef struct {
  uint32_t s_inodes_count;
  uint32_t s_blocks_count;
  uint32_t s_r_blocks_count;
  uint32_t s_free_blocks_count;
  uint32_t s_free_inodes_count;
  uint32_t s_first_data_block;
  uint32_t s_log_block_size;
  int32_t s_log_frag_size;
  uint32_t s_blocks_per_group;
  uint32_t s_frags_per_group;
  uint32_t s_inodes_per_group;
  uint32_t s_mtime;
  uint32_t s_wtime;
  uint16_t s_mnt_count;
  uint16_t s_max_mnt_count;
  uint16_t s_magic;
  uint16_t s_state;
  uint16_t s_errors;
  uint16_t s_minor_rev_level;
  uint32_t s_lastcheck;
  uint32_t s_checkinterval;
  uint32_t s_creator_os;
  uint32_t s_rev_level;
  uint16_t s_def_resuid;
  uint16_t s_def_resgid;
  uint32_t s_first_ino;
  uint16_t s_inode_size;
  uint16_t s_block_group_nr;
  uint32_t s_feature_compat;
  uint32_t s_feature_incompat;
  uint32_t s_feature_ro_compat;
  uint32_t s_uuid[4];
  char s_volume_name[16];
  //uint32_t s_last_mounted[16];
  unsigned char s_last_mounted[64];
  uint32_t s_algo_bitmap;
}Ext;

typedef struct{
    uint32_t bg_block_bitmap;
    uint32_t bg_inode_bitmap;
    uint32_t bg_inode_table;
    uint16_t bg_free_blocks_count;
    uint16_t bg_free_inodes_count;
    uint16_t bg_used_dirs_count;
    uint16_t bg_pad;
    unsigned char bg_reserved[12];
}GroupDescriptor;

typedef struct {
    uint16_t i_mode;        // Mode (type of file and permissions)
    uint16_t i_uid;         // Owner's user ID
    uint32_t i_size;        // File size (least significant 32 bits) <--
    uint32_t i_atime;       // Last access time
    uint32_t i_ctime;       // Creation time
    uint32_t i_mtime;       // Last modification time
    uint32_t i_dtime;       // Deletion time
    uint16_t i_gid;         // Owner's group ID
    uint16_t i_links_count; // Reference counter (number of hard links)
    uint32_t i_blocks;      // Number of 512-byte blocks reserved for this inode
    uint32_t i_flags;       // Flags
    uint32_t i_osd1;        // OS-dependant value
    uint32_t i_block[15];   //Blocs amb informacio <--
    uint32_t i_generation;  // File version (used for NFS)
    uint32_t i_file_acl;    // Block number for extended attributes
    uint32_t i_dir_acl;     // File size (most significant 32 bits)
    uint32_t i_faddr;       // Location of file fragment (deprecated)
    unsigned char i_osd2[12];
} Inode;

typedef struct{
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
}DirectoryEntry;

void EXT_mostraInfoExt2(Ext ext);
Inode EXT_getInode(int fd, Ext ext, int numInode);
int EXT_findFile(int fd, Ext ext, char* fileName, int numInode);

#endif
