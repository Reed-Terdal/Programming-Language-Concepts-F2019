//
// Created by reedt on 10/7/2019.
//

#include "ids.h"

static GHashTable * id_to_type = NULL;

static GHashTable * global_scope = NULL;

void initialize_type_table();

gboolean addIDtoTable(GString * id, Type id_type)
{
    if(id_to_type == NULL)
    {
        initialize_type_table();
    }

    // Assuming that the caller still owns id_type, so we will heap allocate it
    Type * allocType = malloc(sizeof(Type));
    *allocType = id_type;

    return g_hash_table_insert(id_to_type, id->str, allocType);
}

gboolean findIDType(GString * id, Type * ret_type)
{
    if(id_to_type == NULL)
    {
        initialize_type_table();
    }

    gpointer ret_val = g_hash_table_lookup(id_to_type, id->str);
    if(ret_val != NULL)
    {
        *ret_type = *(Type *)ret_val;
        return TRUE;
    }
    return FALSE;
}

void initialize_type_table()
{
    if(id_to_type == NULL)
    {
        id_to_type = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);
        Type * charAtType = malloc(sizeof(Type));
        Type * concatType = malloc(sizeof(Type));
        Type * printType = malloc(sizeof(Type));
        *charAtType = jf_str;
        *concatType = jf_str;
        *printType = jf_void;

        // Add the native bindings, always present
        g_hash_table_insert(id_to_type, "print", printType);
        g_hash_table_insert(id_to_type, "charAt", charAtType);
        g_hash_table_insert(id_to_type, "concat", concatType);
    }
}
