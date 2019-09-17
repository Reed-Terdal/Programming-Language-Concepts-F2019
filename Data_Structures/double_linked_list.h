//
// Created by reedt on 9/17/2019.
//

#ifndef JOTT_INTERPRETER_DOUBLE_LINKED_LIST_H
#define JOTT_INTERPRETER_DOUBLE_LINKED_LIST_H

typedef struct DoublyLinkedListElem
{
    void * data;
    struct DoubleLinkedList * head;
    struct DoubleLinkedList * prev;
} DoublyLinkedListElem;

typedef struct DoubleLinkedList
{
    DoublyLinkedListElem * head;
    void (* insertAt)(struct DoubleLinkedList *, void * data, int location);
    void (* insertHead)(struct DoubleLinkedList *, void * data);
    void (* insertTail)(struct DoubleLinkedList *, void * data);
    struct DoubleLinkedList * (* join)(struct DoubleLinkedList *, struct DoubleLinkedList *);
    void (* removeAt)(struct DoubleLinkedList *, int location);
    void (* removeAllBy)(struct DoubleLinkedList *, char (*filter)(void *));
    unsigned int (* size)(struct DoubleLinkedList *);
    void * (* get)(struct DoubleLinkedList *, int location);
    void * (* findFirstBy)(struct DoubleLinkedList *, char (*filter)(void *));
    void * (* findLastBy)(struct DoubleLinkedList *, char (*filter)(void *));
    struct DoubleLinkedList * (* findAllBy)(struct DoubleLinkedList *, char (*filter)(void *));
} DoubleLinkedList;

#endif //JOTT_INTERPRETER_DOUBLE_LINKED_LIST_H
