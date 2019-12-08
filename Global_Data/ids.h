/**************************************************************************************************
*
* File Name: ids.h
*
* Author: Reed Terdal
*
* Created for CS344 on: 10/07/2019
*
* Purpose: Header that is responsible for providing access to functions relating to the
 * type system and runtime variables for Jott.
*
**************************************************************************************************/


#ifndef JOTT_INTERPRETTER_IDS_H
#define JOTT_INTERPRETTER_IDS_H

#include <glib.h>
#include "p_list.h"


static GHashTable *global_type_table = NULL;

static GHashTable *global_scope = NULL;

static GHashTable *function_prototypes = NULL;

static GHashTable *function_type_tables = NULL;

static GHashTable *active_function_type_table = NULL;
static GString *active_function_type_table_name = NULL;

static GQueue *runtime_scope_stack = NULL;
/**
 * This enum defines all the values that an ID can have.
 */
typedef enum Type
{
    jt_INVALID = 0, // Invalid ID, should not occur in normal use.
    jf_void, // Void function, this ID represents a function that will not return anything, IE print.
    jf_str, // String function, this ID represents a function that will return a String on execution.
    jf_double, // Double function, this ID represents a function that will return a Double on execution.
    jf_int, // Integer function, this ID represents a function that will return an Integer on execution.
    jint, // Integer, this ID represents a literal integer value.
    jdouble, // Double, this ID represents a literal integer value.
    jstring // String, this ID represents a literal string value.
}Type;

/**
 * Each user-defined function should have named parameters, when adding a function prototype the GArray should contain these elements so that the params can be added to the type table automatically
 */
typedef struct named_parameter
{
    GString * name;
    Type type;
}named_parameter;

/**
 * This struct represents a single runtime variable that will be stored in either the global scope or a local scope.
 */
typedef struct runtime_variable
{
    Type type; // The type of the variable.
    void * value; // The data of the variable (int64 *, double *, GString *)
} runtime_variable;

/**
 * @brief This function adds an ID to the type table during parsing/syntax analysis.
 * @param ID The id (string) of the new ID.
 * @param type The type of the new ID.
 * @return True if the ID is unique (not already in the table) or false if otherwise (this should raise an error in the caller).
 */
gboolean addIDtoTable(GString * ID, Type type);

/**
 * @brief This function looks up the type of a given ID.
 * @param ID The ID to search the type table for.
 * @param type[OUT] This pointer will be filled with the ID type if the ID is found.
 * @return True if the ID was found or false otherwise (this should raise an error in the caller).
 */
gboolean findIDType(GString * ID, Type * type);

/**
 * @brief This function updates the value of a runtime variable in the current scope.
 * @param id The ID to set the value for.
 * @param value The value to set for the ID.
 */
void setRuntimeVariable(GString * id, void * value);

/**
 * @brief This gets the current value of a runtime variable in the current scope.
 * @param id The ID of the variable to get the value for.
 * @return The value of the given ID.
 */
runtime_variable * getRuntimeVariable(GString * id);

/**
 * @brief This is a clean up utility that frees all the memory associated with runtime variables in the Global scope.
 */
void destroyRuntimeScopes();

/**
 * @brief Used to create a new function. Used IN PLACE OF addIDToTable for functions specifically
 * @param function_id The id of the function to create a new prototype for
 * @param types[IN] A GArray which contains `named_parameter` members (see above).
 *                  It should only use jint, jdouble, jstring, and jt_INVALID (for wildcards, ie print)
 * @param retval The return type of the function, should be jf_int, jf_double, jf_str and jf_void
 * @note This will exit with an error if the function_id already exists, or if there is a bad `Type` in the types array.
 *                  A valid array should always be passed in, it can be empty if no parameters are required.
 *                  This function will take ownership of the types parameter, it will be freed after execution is complete.
 */
void addFunctionPrototype(GString * function_id, GArray * types, Type retval);

/**
 * @brief This is used to validate the parameters to a function call.
 * @param function_id The ID of the function being called
 * @param params The p_list that will be passed into the function call
 * @note This will exit with an error if the function_id does not exist or if the params does not match the prototype
 */
void checkFunctionParameters(GString * function_id, p_list * params);

/**
 * @brief Used to switch the current type table to be function scope first and then global scope
 * @param function_id The ID of the function being entered
 * @note Used exclusively during the parsing phase.
 *       Will exit with an error if the function_id does not exist yet or if it is already in another function scope.
 * @warning Should ALWAYS be followed by a call to `parsing_enter_function_scope` when done parsing the function body.
 */
void parsing_enter_function_scope(GString * function_id);

/**
 * @brief Used to exit the local scoped type table of a function during parsing
 * @note Used exclusively during the parsing phase.
 *       Will exit with an error if it was not currently in a function scope.
 */
void parsing_exit_function_scope();

/**
 * @brief Used during runtime to create a new scope of local variables in a function call
 * @param function_id The ID of the function being entered
 * @param paramValues A GArray pointer containing void pointers to the values for the parameters to a function
 * @note Used exclusively during the execution phase.
 *       Can be called repeatedly before calling `runtime_exit_function_scope`.
 *       Will exit with an error if the function_id does not exist.
 *       Should always be followed by a call to `runtime_exit_function_scope` after the function returns.
 */
void runtime_enter_function_scope(GString * function_id, GArray * paramValues);

/**
 * @brief Used during runtime to exit a function scope and free any runtime variables belonging to it.
 * @note Used exclusively during the execution phase.
 *       Will exit with an error if not already in a function scope.
 */
void runtime_exit_function_scope();

/**
 * @brief This is a clean up utility called after execution to free all type tables and function prototype information.
 */
void destroy_type_tables();

gboolean in_function_scope();

gpointer get_function(GString *id);

#endif //JOTT_INTERPRETTER_IDS_H
