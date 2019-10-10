//
// Created by reedt on 10/7/2019.
//

#ifndef JOTT_INTERPRETTER_IDS_H
#define JOTT_INTERPRETTER_IDS_H

#include <glib.h>

typedef enum Type
{
    jt_INVALID = 0,
    jf_void,
    jf_str,
    jf_double,
    jf_int,
    jint,
    jdouble,
    jstring
}Type;

typedef struct runtime_variable
{
    Type type;
    void * value;
} runtime_variable;

gboolean addIDtoTable(GString *, Type);

gboolean findIDType(GString *, Type *);

void setGlobalVariable(GString * id, void * value);

runtime_variable * getGlobalVariable(GString * id);

void destroyGlobalScope();

#endif //JOTT_INTERPRETTER_IDS_H
