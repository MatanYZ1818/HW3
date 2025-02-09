/**************/
/*   list.c   */
/**************/


#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#include "ShoppingItem.h"


//////////////////////////////////////////
// Init
// Aim:		create new list
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
//////////////////////////////////////////
BOOL L_init(LIST *pList) {
    if (pList == NULL)
        return False;    // no list to initialize

    pList->head.next = NULL;
    return True;
}


/////////////////////////////////////////////////////////////////
// Insert
// Aim:		add new node
// Input:	pointer to the node BEFORE the place for the new one
//			a value to be stored in the new node
// Output:	pointer to the new node
/////////////////////////////////////////////////////////////////
NODE *L_insert(NODE *pNode, DATA Value) {
    NODE *tmp;

    if (!pNode)
        return NULL;

    tmp = (NODE *) malloc(sizeof(NODE));    // new node

    if (tmp != NULL) {
        tmp->key = Value;
        tmp->next = pNode->next;
        pNode->next = tmp;
    }
    return tmp;
}


//////////////////////////////////////////////////////////////
// Delete
// Aim:		erase node
// Input:	pointer to the node BEFORE the node to be deleted 
// Output:	TRUE if succeeded
//////////////////////////////////////////////////////////////
BOOL L_delete(NODE *pNode) {
    NODE *tmp;
    if (!pNode)
        return False;
    tmp = pNode->next;
    if (!tmp)
        return False;

    pNode->next = tmp->next;
    free(tmp);
    return True;
}


/////////////////////////////////////////////////////////
// Find
// Aim:		search for a value
// Input:	pointer to the node to start with 
//			a value to be found
// Output:	pointer to the node containing the Value
/////////////////////////////////////////////////////////
const NODE *L_find(const NODE *pNode, DATA Value) {
    const NODE *tmp = pNode;

    while (tmp != NULL) {
        if (tmp->key == Value)
            return tmp;
        tmp = tmp->next;
    }

    return NULL;
}


////////////////////////////////////////////////
// Free (additional function)
// Aim:		free the list memory
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
////////////////////////////////////////////////
BOOL L_free(LIST *pList) {
    NODE *tmp;
    BOOL cont = True;
    if (!pList)
        return False;

    tmp = &(pList->head);
    while (cont)
        cont = L_delete(tmp);

    return True;
}


////////////////////////////////////////////////
// Print (additional function)
// Aim:		print the list content
// Input:	pointer to the list structure and a pointer the print function of the specific DATA type
// Output:	a number of the printed elements
////////////////////////////////////////////////
int L_print(const LIST *pList, void (*printElem)(DATA const)) {
    NODE *tmp;
    int c = 0;

    if (!pList)
        return False;

    printf("\n");
    tmp = pList->head.next;
    while (tmp != NULL) {
        printElem((char*)tmp->key);
        c++;
        tmp = tmp->next;
    }
    printf("\n");
    return c;
}

void L_printItem(DATA const pItem)
{
    printItem(pItem);
}

void L_printInt(DATA const pInt)
{
    printf(" %d ---> ", *(int*)pInt);
}

void L_printFloat(DATA const pFloat)
{
    printf(" %.2f ---> ", *(float*)pFloat);
}

void L_printChar(DATA const pChar)
{
    printf(" %c ---> ", *(char*)pChar);
}

void L_printString(DATA const pString)
{
    printf(" %s ---> ", (char*)pString);
}