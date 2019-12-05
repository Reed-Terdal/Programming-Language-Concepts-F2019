/**************************************************************************************************
*
* File Name: ids.c
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/07/2019
*
* Purpose: Functions that are responsible for providing access to the type system and runtime variables for Jott.
*
**************************************************************************************************/

#include <stdio.h>
#include "ids.h"
#include "expr.h"

static GHashTable *global_type_table = NULL;

static GHashTable *global_scope = NULL;

static GHashTable *function_prototypes = NULL;

static GHashTable *function_type_tables = NULL;

static GHashTable *active_function_type_table = NULL;
static GString *active_function_type_table_name = NULL;

static GQueue *runtime_scope_stack = NULL;

typedef struct named_scope{
    GString * name;
    GHashTable *local_scope;
} named_scope;

void initialize_type_table();

gboolean addIDtoTable(GString *id, Type id_type)
{
    if (global_type_table == NULL)
    {
        initialize_type_table();
    }

    // Assuming that the caller still owns id_type, so we will heap allocate it
    Type *allocType = malloc(sizeof(Type));
    *allocType = id_type;

    if(active_function_type_table != NULL)
    {
        // We are in a function, so add it to that type table
        return g_hash_table_insert(active_function_type_table, id->str, allocType);
    }
    else
    {
        // Global scope
        return g_hash_table_insert(global_type_table, id->str, allocType);
    }
}

gboolean findIDType(GString *id, Type *ret_type)
{
    if (global_type_table == NULL)
    {
        initialize_type_table();
    }

    gpointer ret_val = NULL;
    if(active_function_type_table != NULL)
    {
        // Inside a function scope, check that type table first
        ret_val = g_hash_table_lookup(active_function_type_table, id->str);
    }

    if(ret_val == NULL)
    {
        // not in function scope or id is global
        ret_val = g_hash_table_lookup(global_type_table, id->str);
    }

    if (ret_val != NULL)
    {
        if(ret_type != NULL)
        {
            *ret_type = *(Type *) ret_val;
        }
        return TRUE;
    }
    return FALSE;
}

/**
 * @brief This function is responsible for doing the initialization for the type table.
 * This involves allocating the hash table and adding in all natively bound functions.
 */
void initialize_type_table()
{
    if (global_type_table == NULL)
    {
        global_type_table = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);
        Type *charAtType = malloc(sizeof(Type));
        Type *concatType = malloc(sizeof(Type));
        Type *printType = malloc(sizeof(Type));
        *charAtType = jf_str;
        *concatType = jf_str;
        *printType = jf_void;

        // Add the native bindings, always present
        g_hash_table_insert(global_type_table, "print", printType);
        g_hash_table_insert(global_type_table, "charAt", charAtType);
        g_hash_table_insert(global_type_table, "concat", concatType);
    }
}


void setRuntimeVariable(GString *id, void *value)
{
    if (global_scope == NULL)
    {
        global_scope = g_hash_table_new(g_str_hash, g_str_equal);
    }
    if (id != NULL)
    {
        Type id_type;
        if(!findIDType(id, &id_type))
        {
            fprintf(stderr, "Cannot set variable for ID that does not exist");
            exit(-1);
        }
        named_scope *local_scope = g_queue_peek_head(runtime_scope_stack);
        GHashTable *table_to_update = global_scope;

        if(local_scope != NULL)
        {
            // We are in a function
            if(g_hash_table_contains(active_function_type_table, id->str))
            {
                // The ID is in the function
                table_to_update = local_scope->local_scope;
            }
        }

        runtime_variable * old_val = g_hash_table_lookup(table_to_update, id->str);
        if(old_val != NULL)
        {
            // Has already been set, just need to update it
            if(id_type == jstring)
            {
                g_string_free(old_val->value, TRUE);
            }
            else
            {
                free(old_val->value);
            }
            old_val->value = value;
        }
        else
        {
            runtime_variable *new_runtime_var = calloc(1, sizeof(runtime_variable));
            new_runtime_var->type = id_type;
            new_runtime_var->value = value;
            g_hash_table_insert(table_to_update, id->str, new_runtime_var);
        }
    }
}

runtime_variable * getRuntimeVariable(GString * id)
{
    runtime_variable * retval = NULL;
    if (global_scope == NULL)
    {
        global_scope = g_hash_table_new(g_str_hash, g_str_equal);
    }
    if(id != NULL)
    {
        named_scope * current_scope = g_queue_peek_head(runtime_scope_stack);
        GHashTable *table_to_use = global_scope;

        if(current_scope != NULL)
        {
            if(g_hash_table_contains(current_scope->local_scope, id->str))
            {
                table_to_use = current_scope->local_scope;
            }
        }

        retval = g_hash_table_lookup(table_to_use, id->str);
    }
    return retval;
}

void destroyRuntimeScopes()
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

void initializeFunctionPrototypes()
{
    if(function_prototypes == NULL)
    {
        function_prototypes = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);

        GArray * builtin_proto = g_array_new(true, true, sizeof(named_parameter));
        named_parameter param = {NULL, jt_INVALID};
        g_array_append_val(builtin_proto, param);
        g_hash_table_insert(function_prototypes, "print", builtin_proto);

        builtin_proto = g_array_new(true, true, sizeof(Type));
        param.type = jstring;
        g_array_append_val(builtin_proto, param);
        g_array_append_val(builtin_proto, param);
        g_hash_table_insert(function_prototypes, "concat", builtin_proto);

        builtin_proto = g_array_new(true, true, sizeof(Type));
        param.type = jint;
        g_array_append_val(builtin_proto, param);
        param.type = jstring;
        g_array_append_val(builtin_proto, param);
        g_hash_table_insert(function_prototypes, "charAt", builtin_proto);
    }
}

void addFunctionPrototype(GString * function_id, GArray * types, Type retval)
{
    if(function_prototypes == NULL)
    {
        initializeFunctionPrototypes();
    }

    if(function_id == NULL || types == NULL)
    {
        fprintf(stderr, "Parsing Error: function ID and types array cannot be null when adding function prototype");
        exit(-1);
    }

    if(findIDType(function_id, NULL) || g_hash_table_contains(function_prototypes, function_id->str))
    {
        // The ID is already in use, throw an error
        fprintf(stderr, "Parsing Error: ID cannot be used for function as it is already in use");
        exit(-1);
    }

    g_hash_table_insert(function_prototypes, function_id->str, types);
    addIDtoTable(function_id, retval);
}


void checkFunctionParameters(GString * function_id, p_list * params)
{
    if(function_prototypes == NULL)
    {
        initializeFunctionPrototypes();
    }

    if(function_id == NULL || params == NULL)
    {
        fprintf(stderr, "Parsing Error: function ID and params cannot be null when checking function call");
        exit(-1);
    }

    Type varType;
    if(!findIDType(function_id, &varType))
    {
        fprintf(stderr, "Tried to check function prototype for non-existent ID");
        exit(-1);
    }

    if(varType != jf_double && varType != jf_int && varType != jf_str && varType != jf_void)
    {
        fprintf(stderr, "Tried to check function prototype for non-function ID");
        exit(-1);
    }

    GArray * prototype = g_hash_table_lookup(function_prototypes, function_id->str);
    if(prototype == NULL)
    {
        fprintf(stderr, "Could not find function prototype for given function ID");
        exit(-1);
    }

    // Need to walk the params linked-list and confirm the prototype
    p_list * current = params;
    for(int idx = 0; idx < prototype->len; idx++)
    {
        if(current == NULL)
        {
            fprintf(stderr, "Function call does not have enough parameters for prototype");
            exit(-1);
        }

        Type expt = g_array_index(prototype, named_parameter , idx).type;
        if(current->expression->int_expression != NULL)
        {
            if(expt != jint && expt != jt_INVALID)
            {
                fprintf(stderr, "Parsing Error: Function call does not match prototype");
                exit(-1);
            }
        }
        else if(current->expression->double_expression != NULL)
        {
            if(expt != jdouble && expt != jt_INVALID)
            {
                fprintf(stderr, "Parsing Error: Function call does not match prototype");
                exit(-1);
            }
        }
        else if (current->expression->string_expression != NULL)
        {
            if(expt != jstring && expt != jt_INVALID)
            {
                fprintf(stderr, "Parsing Error: Function call does not match prototype");
                exit(-1);
            }
        }
        else
        {
            fprintf(stderr, "Parsing Error: Param contains invalid expression");
        }
        current = current->next;
    }
    if(current != NULL)
    {
        fprintf(stderr, "Function call has too many parameters for prototype");
    }
}


void parsing_enter_function_scope(GString * function_id)
{
    if(function_prototypes == NULL)
    {
        initializeFunctionPrototypes();
    }

    if(function_id == NULL)
    {
        fprintf(stderr, "Parsing Error: function ID cannot be null when entering function type table");
        exit(-1);
    }

    Type varType;
    if(!findIDType(function_id, &varType))
    {
        fprintf(stderr, "Tried to enter function scope for non-existent ID");
        exit(-1);
    }

    if(varType != jf_double && varType != jf_int && varType != jf_str && varType != jf_void)
    {
        fprintf(stderr, "Tried to enter function scope for non-function ID");
        exit(-1);
    }

    GArray * prototype = g_hash_table_lookup(function_prototypes, function_id->str);
    if(prototype == NULL)
    {
        fprintf(stderr, "Could not find function prototype for given function ID");
        exit(-1);
    }

    if(active_function_type_table != NULL || active_function_type_table_name != NULL)
    {
        fprintf(stderr, "Tried to enter a new function scope while still in another one. Did you call parsing_exit_function_scope?");
        exit(-1);
    }
    active_function_type_table = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);
    active_function_type_table_name = function_id;
    // Add in the function parameters
    if(prototype->len > 0)
    {
        for(int idx = 0; idx < prototype->len; idx++)
        {
            if(g_array_index(prototype, named_parameter , idx).name != NULL)
            {
                Type * paramType = malloc(sizeof(Type));
                *paramType = g_array_index(prototype, named_parameter , idx).type;
                g_hash_table_insert(active_function_type_table, g_array_index(prototype, named_parameter , idx).name->str, paramType);
            }
        }
    }
}


void parsing_exit_function_scope()
{
    if(active_function_type_table == NULL || active_function_type_table_name == NULL)
    {
        fprintf(stderr, "Attempted to exit function scope when not currently in function scope");
        exit(-1);
    }
    g_hash_table_insert(function_type_tables, active_function_type_table_name->str, active_function_type_table);
    active_function_type_table_name = NULL;
    active_function_type_table = NULL;
}


void runtime_enter_function_scope(GString * function_id)
{
    if(function_id == NULL)
    {
        fprintf(stderr, "Cannot enter function scope with null function ID");
        exit(-1);
    }

    if(runtime_scope_stack == NULL)
    {
        runtime_scope_stack = g_queue_new();
    }

    GHashTable * new_scope_table = g_hash_table_new(g_str_hash, g_str_equal);
    named_scope * local_scope = malloc(sizeof(named_scope));
    local_scope->name = function_id;
    local_scope->local_scope = new_scope_table;
    g_queue_push_head(runtime_scope_stack, local_scope);

    active_function_type_table = g_hash_table_lookup(function_type_tables, function_id->str);
}


void runtime_exit_function_scope()
{
    if(runtime_scope_stack == NULL)
    {
        fprintf(stderr, "Tried to exit function scope when not currently in a function scope");
        exit(-1);
    }

    named_scope *current_scope = g_queue_pop_head(runtime_scope_stack);
    if(current_scope == NULL)
    {
        fprintf(stderr, "Tried to exit function scope when not currently in a function scope");
        exit(-1);
    }
    gpointer key, value;
    GHashTableIter iter;
    g_hash_table_iter_init(&iter, current_scope->local_scope);
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
    g_hash_table_steal_all(current_scope->local_scope);
    g_hash_table_destroy(current_scope->local_scope);
    free(current_scope);
    active_function_type_table = NULL;

    current_scope = g_queue_peek_head(runtime_scope_stack);
    if(current_scope != NULL)
    {
        active_function_type_table = g_hash_table_lookup(function_type_tables, current_scope->name->str);
    }
}


void destroy_type_tables()
{
    gpointer key, value;
    GHashTableIter iter;
    g_hash_table_iter_init(&iter, function_prototypes);
    while (g_hash_table_iter_next (&iter, &key, &value))
    {
        g_array_free((GArray *) value, TRUE);
    }
    g_hash_table_steal_all(function_prototypes);
    g_hash_table_destroy(function_prototypes);
    function_prototypes = NULL;


    g_hash_table_iter_init(&iter, function_type_tables);
    while (g_hash_table_iter_next (&iter, &key, &value))
    {
        gpointer key2, value2;
        GHashTableIter iter2;
        g_hash_table_iter_init(&iter2, value);
        while (g_hash_table_iter_next (&iter2, &key2, &value2))
        {
            free(value2);
        }
        g_hash_table_steal_all(value);
        g_hash_table_destroy(value);
    }
    g_hash_table_steal_all(function_type_tables);
    g_hash_table_destroy(function_type_tables);
    function_type_tables = NULL;


    named_scope * curScope = g_queue_peek_head(runtime_scope_stack);
    while(curScope != NULL)
    {
        runtime_exit_function_scope();
        curScope = g_queue_peek_head(runtime_scope_stack);
    }
    g_queue_free(runtime_scope_stack);
    runtime_scope_stack = NULL;
}
