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
 * @brief This function updates the value of a runtime variable in the global scope.
 * @param id The ID to set the value for.
 * @param value The value to set for the ID.
 */
void setGlobalVariable(GString * id, void * value);

/**
 * @brief This gets the current value of a runtime variable in the Global scope.
 * @param id The ID of the variable to get the value for.
 * @return The value of the given ID.
 */
runtime_variable * getGlobalVariable(GString * id);

/**
 * @brief This is a clean up utility that frees all the memory associated with runtime variables in the Global scope.
 */
void destroyGlobalScope();

#endif //JOTT_INTERPRETTER_IDS_H
