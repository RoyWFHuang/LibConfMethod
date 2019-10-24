#include "lib_config_method.h"
#include "macro.h"
#include <string.h>

const static tConfigData _g_csg_config_table_astruct[] =
{
    {"UID"              , eLONGLONG ,},
    {"VER"              , eLONGLONG ,},
    {"SIZE"             , eLONGLONG ,},
    {"IS_FILE"          , eINT      ,},
    {"SYNLINK"          , eArray    ,},
    {""       , 0     ,} // end
};


int main()
{
    autofree_tConfigDataGroups tConfigDataGroups config_data_grps;
    autofree_tConfigDataGroups tConfigDataGroups config_data_grps2;
    memset(&config_data_grps, 0, sizeof(tConfigDataGroups));
    memset(&config_data_grps2, 0, sizeof(tConfigDataGroups));
    read_config_from_file("config_file.txt",
        &config_data_grps, _g_csg_config_table_astruct);

    config_data_grps2.num_int = 3;
    config_data_grps2.data_ppstruct = calloc(config_data_grps2.num_int, sizeof(tConfigData*));
    config_data_grps2.data_ppstruct[0] = calloc(1, sizeof(tConfigData));
    strcpy(config_data_grps2.data_ppstruct[0]->name_achar, "eINT");
    config_data_grps2.data_ppstruct[0]->data_type_int = eINT;
    config_data_grps2.data_ppstruct[0]->value_int = 100;

    config_data_grps2.data_ppstruct[1] = calloc(1, sizeof(tConfigData));
    strcpy(config_data_grps2.data_ppstruct[1]->name_achar, "eSTRING");
    config_data_grps2.data_ppstruct[1]->data_type_int = eSTRING;
    strcpyALL(config_data_grps2.data_ppstruct[1]->string_pchar, "write test data");

    config_data_grps2.data_ppstruct[2] = calloc(1, sizeof(tConfigData));
    strcpy(config_data_grps2.data_ppstruct[2]->name_achar, "eArray");
    config_data_grps2.data_ppstruct[2]->data_type_int = eArray;
    config_data_grps2.data_ppstruct[2]->str_pp_num_int = 2;
    config_data_grps2.data_ppstruct[2]->string_ppchar =
        calloc(config_data_grps2.data_ppstruct[2]->str_pp_num_int, sizeof(char*));
    strcpyALL(config_data_grps2.data_ppstruct[2]->string_ppchar[0], "array0");
    strcpyALL(config_data_grps2.data_ppstruct[2]->string_ppchar[1], "array1");
    write_config_file("w_config_file.txt", &config_data_grps2);
    return 0;
}
