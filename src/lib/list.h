/* vim: set tabstop=8 shiftwidth=8: */
//=============================================================================
// Name: list.h
// Purpose: Common Bidirection List
// To build: gcc -std-c99 -c list.c
//=============================================================================

#ifndef _LIST_H
#define _LIST_H

#ifdef __cplusplus
extern "C" {
#endif
#include "if.h"
/*============================================================================
 * Struct Declaration
 ===========================================================================*/
typedef struct _NODE
{
        struct _NODE *next;
        struct _NODE *prev;
        uint32_t key; // for list_insert()
}
NODE;

typedef struct _LIST
{
        NODE *head;
        NODE *tail;
        int count;
}
LIST;

/*============================================================================
 * Public Function Declaration
 ===========================================================================*/
void  list_init(LIST *list);
void  list_free(LIST *list);

void  list_del(LIST *list, NODE *node);
void  list_add(LIST *list, NODE *node, uint32_t key); // to list tail
void  list_insert(LIST *list, NODE *node, uint32_t key); // small key first
NODE *list_search(LIST *list, uint32_t key);

int   list_cnt(LIST *list);
NODE *list_head(LIST *list);
NODE *list_tail(LIST *list);
NODE *list_next(NODE *node);
NODE *list_prev(NODE *node);

#ifdef __cplusplus
}
#endif

#endif // _LIST_H

/*****************************************************************************
 * End
 ****************************************************************************/
