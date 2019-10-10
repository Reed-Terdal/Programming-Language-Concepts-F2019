//
// Created by reedt on 10/7/2019.
//

#include "ids.h"

static GHashTable *id_to_type = NULL;

static GHashTable *global_scope = NULL;

void initialize_type_table();

gboolean addIDtoTable(GString *id, Type id_type)
{
    if (id_to_type == NULL)
    {
        initialize_type_table();
    }

    // Assuming that the caller still owns id_type, so we will heap allocate it
    Type *allocType = malloc(sizeof(Type));
    *allocType = id_type;

    return g_hash_table_insert(id_to_type, id->str, allocType);
}

gboolean findIDType(GString *id, Type *ret_type)
{
    if (id_to_type == NULL)
    {
        initialize_type_table();
    }

    gpointer ret_val = g_hash_table_lookup(id_to_type, id->str);
    if (ret_val != NULL)
    {
        *ret_type = *(Type *) ret_val;
        return TRUE;
    }
    return FALSE;
}

void initialize_type_table()
{
    if (id_to_type == NULL)
    {
        id_to_type = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);
        Type *charAtType = malloc(sizeof(Type));
        Type *concatType = malloc(sizeof(Type));
        Type *printType = malloc(sizeof(Type));
        *charAtType = jf_str;
        *concatType = jf_str;
        *printType = jf_void;

        // Add the native bindings, always present
        g_hash_table_insert(id_to_type, "print", printType);
        g_hash_table_insert(id_to_type, "charAt", charAtType);
        g_hash_table_insert(id_to_type, "concat", concatType);
    }
}


void setGlobalVariable(GString *id, void *value)
{
    if (global_scope == NULL)
    {
        global_scope = g_hash_table_new(g_str_hash, g_str_equal);
    }
    if (id != NULL)
    {
        // We know about the variable from the type table
        runtime_variable *old_val = g_hash_table_lookup(global_scope, id->str);
        if (old_val != NULL)
        {
            // We already have a value for this, update it
            old_val->value = value;
        }
        else
        {
            Type id_type;
            if (findIDType(id, &id_type))
            {
                runtime_variable *new_runtime_var = calloc(1, sizeof(runtime_variable));
                new_runtime_var->type = id_type;
                new_runtime_var->value = value;
                g_hash_table_insert(global_scope, id->str, new_runtime_var);
            }
        }
    }
}

runtime_variable * getGlobalVariable(GString * id)
{
    runtime_variable * retval = NULL;
    if (global_scope == NULL)
    {
        global_scope = g_hash_table_new(g_str_hash, g_str_equal);
    }
    if(id != NULL)
    {
        retval = g_hash_table_lookup(global_scope, id->str);
    }
    return retval;
}

void destroyGlobalScope()
{
    if(global_scope != NULL)
    {
        gpointer key, value;
        GHashTableIter iter;
        g_hash_table_iter_init(&iter, global_scope);
        while (g_hash_table_iter_next (&iter, &key, &value))
        {
            if(((runtime_variable *)value)->value != NULL)
            {
                if(((runtime_variable *)value)->type == jstring)
                {
                    g_string_free(((runtime_variable *)value)->value, TRUE);
                }
                else
                {
                    free(((runtime_variable *)value)->value);
                }
            }
            free(value);
        }
        g_hash_table_steal_all(global_scope);
        g_hash_table_destroy(global_scope);
        global_scope = NULL;
    }
}