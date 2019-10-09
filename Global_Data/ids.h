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

gboolean addIDtoTable(GString *, Type);

gboolean findIDType(GString *, Type *);

#endif //JOTT_INTERPRETTER_IDS_H
