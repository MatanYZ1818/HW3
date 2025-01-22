/********************************************/
/*   list.h                                 */
/*   a dynamic  linked list with a header   */
/********************************************/


#ifndef _LIST_
#define _LIST_

#include "def.h"


/*** Definitions ***/

// List
typedef struct 
{
	NODE head;
}LIST;


/*** Function prototypes ***/

BOOL L_init(LIST* pList);					// create new list

NODE* L_insert(NODE* pNode, DATA Value);	// add new node after *pNode

BOOL L_delete(NODE* pNode);					// erase node after *pNode

const NODE* L_find(const NODE* pNode, DATA Value);		// return a pointer to the node 

BOOL L_free(LIST* pList);					// free list memory

int L_print(const LIST* pList, void (*printElem)(DATA const));					// print the list content

void L_printProduct(DATA const pProduct);

void L_printItem(DATA const pItem);

void L_printInt(DATA const pInt);

void L_printFloat(DATA const pFloat);

void L_printChar(DATA const pChar);

void L_printString(DATA const pString);

#endif
