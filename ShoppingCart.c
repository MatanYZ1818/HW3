#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ShoppingCart.h"
#include "General.h"

int	initCart(ShoppingCart* pCart)
{
	pCart->itemList = (LIST*)malloc(sizeof(LIST));
	return L_init(pCart->itemList);
}

float	getTotalPrice(const ShoppingCart* pCart)
{

    float price = 0;
    NODE *pItem = pCart->itemList->head.next;
    while (pItem != NULL)
    {
        price += ((ShoppingItem*)pItem->key)->price * ((ShoppingItem*)pItem->key)->count;
    	pItem = pItem->next;
    }

	return price;
}

int	addItemToCart(ShoppingCart* pCart, const char* barcode, float price, int count)
{
	ShoppingItem* pItem = getItemByBarocde(pCart, barcode);
	if (!pItem) //new item
	{
		pItem = createItem(barcode, price, count);
		if (!pItem)
			return True;

		NODE *pTemp = &pCart->itemList->head;
		while (pTemp->next != NULL && strcmp(pItem->barcode, ((ShoppingItem*)pTemp->next->key)->barcode) == 1) pTemp = pTemp->next;
		L_insert(pTemp, pItem);
	}
	else {
		pItem->count += count;
	}
	return False;
}

float	printShoppingCart(const ShoppingCart* pCart)
{
	float price = 0;
	L_print(pCart->itemList, L_printItem);
	NODE *pItem = pCart->itemList->head.next;
	while (pItem != NULL)
	{
		price += ((ShoppingItem*)pItem->key)->price;
		pItem = pItem->next;
	}
	printf("Total bill to pay: %.2f\n", price);
	return price;
}



ShoppingItem*	getItemByBarocde(ShoppingCart* pCart, const char* barcode)
{
	NODE *pItem = pCart->itemList->head.next;
	while (pItem != NULL)
	{
		if (strcmp(((ShoppingItem*)pItem->key)->barcode, barcode) == 0)
			return pItem->key;
	}

	return NULL;
}

int	freeShoppingCart(ShoppingCart* pCart)
{
	if (!L_free(pCart->itemList)) return False;
	free(pCart->itemList);
	return True;
}