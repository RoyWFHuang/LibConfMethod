

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>

#include "macro.h"
//#include "common.h"
#include "lib_config_method.h"
#include "define.h"
#include "errorno.h"
//#include "util_lib.h"

// allocate config data memory
#define malloc_One_ConfigData_In_ConfigDataGroups(config_data_index) do{\
    tmp_data_grps_struct.data_ppstruct[config_data_index] = \
        (tConfigData *)calloc(1, sizeof(tConfigData));\
}while(0)

#define write_ConfigData_Name_And_Type(config_data_index, name, type) do{\
    strcpy(tmp_data_grps_struct.data_ppstruct[config_data_index]->name_achar, name);\
    tmp_data_grps_struct.data_ppstruct[config_data_index]->data_type_int = type;\
}while(0)

//tConfigData *_g_config_table_pstruct;

int read_config_from_file(char *file_path,
    tConfigDataGroups *config_data_grps_data,
    const tConfigData *ref_conf_table)
{
    if((NULL == file_path) ||
        (NULL == config_data_grps_data) ||
        (NULL == ref_conf_table))
    {
        return -1;
    }
    int ret_int = 0;

    config_t config_struct;
    config_init(&config_struct);

    if (!config_read_file(&config_struct, file_path))
    {
#ifdef LCM_DEBUG_MODE
        LCM_DEBUG_PRINT("read file not exist %s(%d) - %s\n",
            config_error_file(&config_struct),
            config_error_line(&config_struct),
            config_error_text(&config_struct));
#endif
        ret_int = -1;
        goto read_config_from_file_exit_label;
    }
    ret_int = read_config_from_configt(&config_struct,
        config_data_grps_data, ref_conf_table);

read_config_from_file_exit_label:
    config_destroy(&config_struct);
    return ret_int;
}

int read_config_from_configt(config_t *root_config,
    tConfigDataGroups *config_data_grps_data,
    const tConfigData *ref_conf_table)
{

    config_setting_t *root = config_root_setting(root_config);

    tConfigDataGroups tmp_data_grps_struct;
    memset(&tmp_data_grps_struct, 0, sizeof(tConfigDataGroups));
    tmp_data_grps_struct.num_int = config_setting_length(root);

    tmp_data_grps_struct.data_ppstruct = (tConfigData **)
        calloc(tmp_data_grps_struct.num_int, sizeof(tConfigData *));

    const tConfigData *table_pstruct = NULL;
    int config_data_index = 0;

    //for (table_pstruct = (tConfigData *)_g_config_table_pstruct;
    for (table_pstruct = ref_conf_table;
            0 != strlen(table_pstruct->name_achar) ;
            ++table_pstruct)
    {
        switch(table_pstruct->data_type_int)
        {
            case eSTRING :
            {
#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eSTRING - [%s]\n",
                    table_pstruct->name_achar);
#endif
                const char *string_pchar;
                if (config_lookup_string(
                        root_config,
                        table_pstruct->name_achar,
                        &string_pchar
                    )
                )
                {
                    malloc_One_ConfigData_In_ConfigDataGroups(config_data_index);
                    write_ConfigData_Name_And_Type(
                        config_data_index,
                        table_pstruct->name_achar,
                        eSTRING
                    );
                    // write config value
                    strcpyALL(
                        tmp_data_grps_struct.data_ppstruct[config_data_index]->string_pchar,
                        (char *)string_pchar
                    );
#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eSTRING - [%s]\n",
                    tmp_data_grps_struct.data_ppstruct[config_data_index]->string_pchar);
#endif
                    // next index
                    config_data_index++;
                }
                break;
            }
            case eINT :
            {
#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eINT - [%s]\n",
                    table_pstruct->name_achar);
#endif
                int value_int;
                if (config_lookup_int(
                        root_config,
                        table_pstruct->name_achar,
                        &value_int
                    )
                )
                {
                    // allocate config data memory
                    malloc_One_ConfigData_In_ConfigDataGroups(config_data_index);
                    tConfigData *tmp_conf_data_pstruct =
                        tmp_data_grps_struct.data_ppstruct[config_data_index];
                    write_ConfigData_Name_And_Type(
                        config_data_index,
                        table_pstruct->name_achar,
                        eINT
                    );
                    // write config value
                    tmp_conf_data_pstruct->value_int = value_int;
                    // next index
#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eINT - [%d]\n",
                    tmp_conf_data_pstruct->value_int);
#endif
                    config_data_index++;
                }
                break;
            }

            case eFLOAT :
            {
#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eFLOAT - [%s]\n",
                    table_pstruct->name_achar);
#endif
                double value_float = 0;
                if (config_lookup_float(
                        root_config,
                        table_pstruct->name_achar,
                        &value_float
                    )
                )
                {
                    // allocate config data memory
                    malloc_One_ConfigData_In_ConfigDataGroups(config_data_index);
                    tConfigData *tmp_conf_data_pstruct =
                        tmp_data_grps_struct.data_ppstruct[config_data_index];
                    write_ConfigData_Name_And_Type(
                        config_data_index,
                        table_pstruct->name_achar,
                        eFLOAT
                    );
                    // write config value
                    tmp_conf_data_pstruct->value_float = value_float;
#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eFLOAT - [%f]\n",
                    tmp_conf_data_pstruct->value_float);
#endif
                    // next index
                    config_data_index++;
                }
                break;
            }
            case eLONGLONG :
            {
#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eLONGLONG - [%s]\n",
                    table_pstruct->name_achar);
#endif
                long long value_llong = 0;
                if (config_lookup_int64(
                        root_config,
                        table_pstruct->name_achar,
                        &value_llong)
                )
                {
                    // allocate config data memory
                    malloc_One_ConfigData_In_ConfigDataGroups(config_data_index);
                    tConfigData *tmp_conf_data_pstruct =
                        tmp_data_grps_struct.data_ppstruct[config_data_index];
                    write_ConfigData_Name_And_Type(
                        config_data_index,
                        table_pstruct->name_achar,
                        eLONGLONG
                    );
                    // write config value
                    tmp_conf_data_pstruct->value_llong = value_llong;
#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eLONGLONG - [%lld]\n",
                    tmp_conf_data_pstruct->value_llong);
#endif
                    // next index
                    config_data_index++;
                }
                break;
            }

            case eNEST_STRUCT :
                break;
            case eArray :
            {

#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eArray - [%s]\n",
                    table_pstruct->name_achar);
#endif
                if(NULL == root)
                    break;
                config_setting_t *tmp_setting = config_setting_get_member(
                        root,
                        table_pstruct->name_achar
                );
                if(NULL == tmp_setting)
                    break;

                int e_array_num_int =
                    config_setting_length(tmp_setting);
#ifdef LCM_DEBUG_MODE
                LCM_DEBUG_PRINT("eArray - num = [%d]\n", e_array_num_int);
#endif

                if(e_array_num_int > 0)
                {
                    // allocate config data memory
                    malloc_One_ConfigData_In_ConfigDataGroups(config_data_index);
                    tConfigData *tmp_conf_data_pstruct =
                        tmp_data_grps_struct.data_ppstruct[config_data_index];
                    write_ConfigData_Name_And_Type(
                        config_data_index,
                        table_pstruct->name_achar,
                        eArray
                    );
                    // write config value
                    tmp_conf_data_pstruct->string_ppchar = (char **)
                        calloc(e_array_num_int, sizeof(char *));
                    tmp_conf_data_pstruct->str_pp_num_int = e_array_num_int;

                    for (int i = 0; i < e_array_num_int; i++)
                    {
                        strcpyALL(
                            tmp_conf_data_pstruct->string_ppchar[i],
                            (char *)config_setting_get_string_elem(tmp_setting, i)
                        );
#ifdef LCM_DEBUG_MODE
                        LCM_DEBUG_PRINT("eArray - [%d][%s]\n", i,
                            tmp_conf_data_pstruct->string_ppchar[i]);
#endif
                    }
                    // next index
                    config_data_index++;
                }
                break;
            }
        }
    }

    if(config_data_index != tmp_data_grps_struct.num_int)
    {
        config_data_grps_data->num_int = config_data_index;
        config_data_grps_data->data_ppstruct = (tConfigData **)
            calloc(config_data_index, sizeof(tConfigData *));
        for(int i = 0; i< config_data_index;i++)
            config_data_grps_data->data_ppstruct[i] = tmp_data_grps_struct.data_ppstruct[i];
        free_to_NULL(tmp_data_grps_struct.data_ppstruct);
    }
    else
    {
        config_data_grps_data->num_int = config_data_index;
        config_data_grps_data->data_ppstruct = tmp_data_grps_struct.data_ppstruct;
    }
    return 0;
}


static int write_config_conf(
    config_t *root_config,
    tConfigDataGroups *config_data_grp_pstruct)
{
    config_setting_t *root = config_root_setting(root_config);
    config_setting_t *tmp_setting = NULL;
    int config_index_int = 0;
    while(config_index_int < config_data_grp_pstruct->num_int)
    {
        if( MAX_CONFIG_NUMBER == (config_index_int+1) ) break;
        tConfigData *tmp_conf_data =
            config_data_grp_pstruct->data_ppstruct[config_index_int];
        tmp_setting =
            config_setting_get_member(
                root,
                tmp_conf_data->name_achar
            );

        switch(tmp_conf_data->data_type_int)
        {
            case eSTRING :
                if(tmp_setting == NULL )
                {
                    tmp_setting = config_setting_add(
                        root,
                        tmp_conf_data->name_achar ,
                        CONFIG_TYPE_STRING);
                }
                config_setting_set_string(
                    tmp_setting,
                    tmp_conf_data->string_pchar);
                break;
            case eINT :
                if(tmp_setting == NULL )
                {

                    tmp_setting = config_setting_add(
                        root,
                        tmp_conf_data->name_achar ,
                        CONFIG_TYPE_INT);
                }

                config_setting_set_int(
                    tmp_setting,
                    tmp_conf_data->value_int);
                break;
            case eFLOAT :
                if(NULL == tmp_setting)
                {
                    tmp_setting = config_setting_add(
                        root,
                        tmp_conf_data->name_achar ,
                        CONFIG_TYPE_FLOAT);
                }

                config_setting_set_float(
                    tmp_setting,
                    tmp_conf_data->value_float);
                break;
            case eLONGLONG :
                if(NULL == tmp_setting)
                {

                    tmp_setting = config_setting_add(
                        root,
                        tmp_conf_data->name_achar,
                        CONFIG_TYPE_INT64);
                }

                config_setting_set_int64(
                    tmp_setting,
                    tmp_conf_data->value_llong);

                break;
            case eNEST_STRUCT :
                break;
            case eArray :
            {
                if(NULL == tmp_setting)
                {
                    tmp_setting = config_setting_add(root,
                        tmp_conf_data->name_achar,
                        CONFIG_TYPE_ARRAY
                    );
                }
                int array_num_int = config_setting_length(tmp_setting);

                int8_t flag_int8 = 0;

                for(int str_index_int = 0;
                    str_index_int<tmp_conf_data->str_pp_num_int;
                    str_index_int++)
                {
                    for(int i = 0; i< array_num_int; i++)
                    {
                        if(!strcmp(tmp_conf_data->string_ppchar[str_index_int],
                            config_setting_get_string_elem(tmp_setting, i) ) )
                        {
                            flag_int8 = 1;
                            break;
                        }
                    }
                    if(!flag_int8)
                    {
                        config_setting_t *tmp_conf_setting =
                            config_setting_add(tmp_setting, NULL, CONFIG_TYPE_STRING);
                        config_setting_set_string(
                            tmp_conf_setting,
                            tmp_conf_data->string_ppchar[str_index_int]
                        );
                    }
                }
                //config_setting_set_string_elem(
                //    tmp_setting,
                //    array_num_int,
                //    tmp_conf_data->string_pchar);
                break;
            }
        }
        config_index_int++;
    }


    return 0;
}

int write_config_file(char *file_path, tConfigDataGroups *config_data_grp_pstruct)
{
    if((NULL == file_path) || (NULL == config_data_grp_pstruct))
        return ERROR_CODE_NULL_POINT_EXCEPTION;
    int ret_int = 0;

    config_t config_struct;
    config_init(&config_struct);

    if (!config_read_file(&config_struct, file_path))
    {
        LCM_DEBUG_PRINT("Read config file not exist: %s(%d) - %s\n",
            config_error_file(&config_struct),
            config_error_line(&config_struct),
            config_error_text(&config_struct)
        );
    }

    if(0 != write_config_conf(&config_struct, config_data_grp_pstruct))
    {
        ret_int = -1;
        goto write_config_file_exit_label;
    }

    if(!config_write_file(&config_struct, file_path))
    {
        LCM_ERR_PRINT("Error while writing config file(%s).\n", file_path);
        ret_int = ERROR_CODE_PATH_NOT_EXIST;
    }
write_config_file_exit_label :
    config_destroy(&config_struct);
    return ret_int;
}




int free_config_data(tConfigData *config_data_pstruct)
{
    switch(config_data_pstruct->data_type_int)
    {
        case eSTRING :
        {
            free_to_NULL(config_data_pstruct->string_pchar);
            break;
        }
        case eINT:
        case eFLOAT:
        case eLONGLONG:
        case eNEST_STRUCT:
            break;
        case eArray:
        {
            for(int arr_index_int = 0;
                arr_index_int < config_data_pstruct->str_pp_num_int;
                arr_index_int++)
            {
                free_to_NULL(config_data_pstruct->string_ppchar[arr_index_int]);
                //config_data_pstruct->string_ppchar[arr_index_int] = NULL;
            }
            free_to_NULL(config_data_pstruct->string_ppchar);
            break;
        }
    }
    return 0;
}

int free_config_group(
    tConfigDataGroups *conf_data_struct)
{
    for(int i = 0; i < conf_data_struct->num_int; i++)
    {
        free_config_data(conf_data_struct->data_ppstruct[i]);
        free_to_NULL(conf_data_struct->data_ppstruct[i]);
    }
    free_to_NULL(conf_data_struct->data_ppstruct);
    return 0;
}


int copyConfigDataGroups(
    tConfigDataGroups *dest_pstruct,
    tConfigDataGroups *src_pstruct,
    int start_index_int)
{
    if((NULL == src_pstruct) || (NULL == dest_pstruct))
        return -1;
    if(dest_pstruct->num_int < (start_index_int + src_pstruct->num_int) )
        return -1;
    for(int i = 0; i < src_pstruct->num_int; i++)
    {
        dest_pstruct->data_ppstruct[start_index_int] = (tConfigData*)
            calloc(1, sizeof(tConfigData));
        switch(src_pstruct->data_ppstruct[i]->data_type_int)
        {
            case eINT:
            case eFLOAT:
            case eLONGLONG:
                memcpy(dest_pstruct->data_ppstruct[start_index_int],
                    src_pstruct->data_ppstruct[i],
                    sizeof(tConfigData)
                );
                break;
            case eNEST_STRUCT:
                break;
            case eSTRING:
                memcpy(dest_pstruct->data_ppstruct[start_index_int],
                    src_pstruct->data_ppstruct[i],
                    sizeof(tConfigData)
                );
                dest_pstruct->data_ppstruct[start_index_int]->string_pchar = NULL;

                strcpyALL(
                    dest_pstruct->data_ppstruct[start_index_int]->string_pchar,
                    src_pstruct->data_ppstruct[i]->string_pchar
                );
                break;
            case eArray:
                memcpy(dest_pstruct->data_ppstruct[start_index_int],
                    src_pstruct->data_ppstruct[i],
                    sizeof(tConfigData)
                );
                dest_pstruct->data_ppstruct[start_index_int]->string_ppchar = NULL;
                dest_pstruct->data_ppstruct[start_index_int]->string_ppchar = (char **)
                    calloc(src_pstruct->data_ppstruct[i]->str_pp_num_int, sizeof(char*));

                for(int j = 0; j < src_pstruct->data_ppstruct[i]->str_pp_num_int; j++)
                {
                    strcpyALL(dest_pstruct->data_ppstruct[start_index_int]->string_ppchar[j],
                        src_pstruct->data_ppstruct[i]->string_ppchar[j]
                    );
                }
                break;
        }
        start_index_int++;
    }
    return 0;
}


int modify_config(char *file_path, tConfigDataGroups *config_data_grp_pstruct)
{
    if((NULL == file_path) || (NULL == config_data_grp_pstruct))
        return -1;
    remove(file_path);
    return write_config_file(file_path, config_data_grp_pstruct);
}

#if 0
int read_one_config(
    char *file_path,
    tConfigData *input_pstruct)
{
    if((NULL == file_path) || (NULL == input_pstruct))
        return -1;


    int index_int = -1;
    for (int i = 0; _g_config_table_pstruct[i].name_achar; i++)
    {
        if( !strcmp(input_pstruct->name_achar, _g_config_table_pstruct[i].name_achar) &&
            (_g_config_table_pstruct[i].data_type_int == input_pstruct->data_type_int)
        )
        {
            index_int = i;
            break;
        }
    }

    if(-1 == index_int)
        return 0;


    int ret_int = 0;
    config_t config_struct;
    config_init(&config_struct);

    if (!config_read_file(&config_struct, file_path))
    {
        LCM_ERR_PRINT("Error file %s(%d) - %s\n", config_error_file(&config_struct),
            config_error_line(&config_struct), config_error_text(&config_struct));
        csglog_err("Error file %s(%d) - %s\n", config_error_file(&config_struct),
            config_error_line(&config_struct), config_error_text(&config_struct));
        ret_int = -1;
        goto read_one_config_exit_label;
    }

    switch(input_pstruct->data_type_int)
    {
        case eSTRING:
        {
            const char *string_pchar;
            if (config_lookup_string(&config_struct,
                    input_pstruct->name_achar,
                    &string_pchar)
            ) strcpyALL(input_pstruct->string_pchar, (char *)string_pchar);

            break;
        }
        case eINT:
        {
            int value_int;
            if (config_lookup_int(&config_struct,
                    input_pstruct->name_achar,
                    &value_int)
            ) input_pstruct->value_int = value_int;

            break;
        }
        case eFLOAT:
        {
            double value_float = 0;
            if (config_lookup_float(&config_struct,
                    input_pstruct->name_achar,
                    &value_float)
            ) input_pstruct->value_float = value_float;

            break;
        }
        case eLONGLONG:
        {
            long long value_llong = 0;
            if (config_lookup_int64(&config_struct,
                    input_pstruct->name_achar,
                    &value_llong)
            ) input_pstruct->value_llong = value_llong;

            break;
        }
        case eNEST_STRUCT:
            break;
        case eArray :
        {
            config_setting_t *root = config_root_setting(&config_struct);
            if(NULL == root)
                break;

            config_setting_t *tmp_setting = config_setting_get_member(
                    root,
                    input_pstruct->name_achar
            );
            if(NULL == tmp_setting)
                break;

            int array_num_int = config_setting_length(tmp_setting);
            input_pstruct->str_pp_num_int = array_num_int;
            if(array_num_int > 0)
            {
                //// write config value
                input_pstruct->string_ppchar = malloc(array_num_int*sizeof(char **));
                for (int i = 0; i < array_num_int; i++)
                {
                    strcpyALL(
                        input_pstruct->string_ppchar[i],
                        (char *)config_setting_get_string_elem(tmp_setting, i)
                    );

                }
            }
            break;
        }
    }

read_one_config_exit_label :
    config_destroy(&config_struct);
    return ret_int;
}



#endif

