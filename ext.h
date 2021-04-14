#ifndef _EXT_H_
#define _EXT_H_


typedef struct {
  uint32_t s_inodes_count; //Alomejor son todos int pk 4bytes*8 = 32bits
  uint32_t s_blocks_count;
  uint32_t s_r_blocks_count;
  uint32_t s_free_blocks_count;
  uint32_t s_free_inodes_count;
  uint32_t s_first_data_block;
  uint32_t s_log_block_size; //Nose se si uint16 (especifica solo numeros positivos)
  int32_t s_log_frag_size; //Especificaque pilla valores negativos y positivos
  uint32_t s_blocks_per_group;
  uint32_t s_frags_per_group;
  uint32_t s_inodes_per_group;
  uint32_t s_mtime; //Guarda  fecha --> int64_t (otra prueba)
  uint32_t s_wtime; //Lo mismo q arriba --time_t
  uint16_t s_mnt_count; //Guarda un tiempo (no se si int)
  uint16_t s_max_mnt_count; //Puede q int
  uint16_t magic;
  uint16_t s_state; //Si falla mirar tabla
  uint16_t s_errors; //Lo mismo que arriba
  uint16_t s_minor_rev_level; //Alomejor int
  uint32_t s_lastcheck; //Lo mismo q arriba --time_t
  uint32_t s_checkinterval; //Lo mismo q arriba --time_t
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
  uint128_t s_uuid; //128bit unico k esta bien creojeje
  uint128_t s_volume_name;
  uint512_t s_last_mounted;
  uint32_t s_algo_bitmap; //32bit creo k d los pocos k bnn
  uint8_t s_prealloc_blocks; //Creo que asi tiene que ser :)
  uint8_t s_prealloc_dir_blocks; //Probar
  uint16_t s_journal_uuid; //Probar
  int32_t s_journal_inum; //Probar
  int32_t s_journal_dev;
  int32_t s_last_orphan;
  int32_t s_hash_seed[4]; //Probar
  int8_t s_def_hash_version; //Probar
  int32_t s_default_mount_options;
  int32_t s_first_meta_bg; //32bit
}Ext;










#endif
