#ifndef __lib_config_method_H__
#define __lib_config_method_H__

#include <libconfig.h>

#ifdef CONSOLE_DEBUG
#include <stdio.h>
#   define LCM_DEBUG_PRINT(fmt, str...) \
        printf(\
        "%16.16s(%4d) - %16.16s: " fmt, \
        __FILE__, __LINE__, __func__, ##str)
#   define LCM_ERR_PRINT(fmt, str...) \
        printf(\
        "%16.16s(%4d) - %16.16s: ERROR " fmt, \
        __FILE__, __LINE__, __func__, ##str)
#else
#   include "define.h"
#   define LCM_DEBUG_PRINT PRINT_DBG
#   define LCM_ERR_PRINT PRINT_ERR
#endif

typedef enum _eConfigDataType
{
    eSTRING,
    eINT,
    eFLOAT,
    eLONGLONG,
    eNEST_STRUCT,
    eArray
}eConfigDataType;


typedef struct _tConfigData
{
    char name_achar[64];

    eConfigDataType data_type_int;
    int str_pp_num_int;
    union {
        char *string_pchar;
        int value_int;
        double value_float;
        long long value_llong;
        void *nest_struct_pvoid;
        char **string_ppchar;
    };
}tConfigData;

typedef struct _tConfigDataGroups
{
    int num_int;
    tConfigData **data_ppstruct;
}tConfigDataGroups;

#define MAX_CONFIG_NUMBER 64

int read_config_from_file(char *file_pchar,
    tConfigDataGroups *config_data_grps_pstruct,
    const tConfigData *configtable_pstruct);

int read_config_from_configt(config_t *root_config,
    tConfigDataGroups *config_data_grps_pstruct,
    const tConfigData *configtable_pstruct);

int write_config_file(
    char *file_pchar,
    tConfigDataGroups *config_data_grp_pstruct);

int copyConfigDataGroups(
    tConfigDataGroups *src_pstruct,
    tConfigDataGroups *dest_pstruct,
    int start_index_int);

int modify_config(
    char *file_pchar,
    tConfigDataGroups *config_data_grp_pstruct);

int free_config_data(
    tConfigData *config_data_pstruct);

int free_config_group(
    tConfigDataGroups *conf_data_struct);

#define autofree_tConfigDataGroups \
    __attribute__((cleanup(free_config_group)))

#define autofree_tConfigData \
    __attribute__((cleanup(free_config_data)))

#endif

