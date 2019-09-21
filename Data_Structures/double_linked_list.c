//
// Created by reedt on 9/17/2019.
//

#include <stddef.h>
#include "double_linked_list.h"
#include <stdlib.h>

void insertAt(DoubleLinkedList * this, void * data, int location)
{

}
void insertHead(DoubleLinkedList * this, void * data)
{

}
void insertTail(DoubleLinkedList * this, void * data)
{

}
struct DoubleLinkedList * join(DoubleLinkedList * this, struct DoubleLinkedList * other)
{

}
void * removeAt(DoubleLinkedList * this, int location)
{

}
void * removeAllBy(DoubleLinkedList * this, char (*filter)(void *))
{

}
unsigned int * size(DoubleLinkedList * this)
{

}
void * get(DoubleLinkedList * this, int location)
{

}
void * findFirstBy(DoubleLinkedList * this, char (*filter)(void *))
{

}
void * findLastBy(DoubleLinkedList * this, char (*filter)(void *))
{

}
struct DoubleLinkedList * findAllBy(DoubleLinkedList * this, char (*filter)(void *))
{

}


struct DoubleLinkedList * newDoubleLinkedList()
{
    struct DoubleLinkedList * retVal = malloc(sizeof(struct DoubleLinkedList));
    if(retVal != NULL)
    {

    }
    return NULL;
}
