#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ShoppingCart.h"
#include "General.h"

int	initCart(ShoppingCart* pCart)
{
	pCart->itemList = (LIST*)malloc(sizeof(LIST));
}

float	getTotalPrice(const ShoppingCart* pCart)
{
    /*
	float price = 0;
	ShoppingItem* pItem;
	for (int i = 0; i < pCart->count; i++)
	{
		pItem = pCart->itemArr[i];
		price += (pItem->price * pItem->count);
	}
    */

    float price = 0;
    NODE *pItem = pCart->itemList->head.next;
    while (pItem != NULL)
    {
        price += *(ShoppingItem*)pItem->key * pItem.;
    }

	return price;
}

int		addItemToCart(ShoppingCart* pCart, const char* barcode, float price, int count)
{
	ShoppingItem* pItem = getItemByBarocde(pCart, barcode);
	if (!pItem) //new item
	{
		pItem = createItem(barcode, price, count);
		if (!pItem)
			return 0;

		ShoppingItem** tempArr = (ShoppingItem**)realloc(pCart->itemArr, (pCart->count + 1) * sizeof(ShoppingItem*));
		if (!tempArr) {
			freeShoppingCart(pCart);
			return 0;
		}
		pCart->itemArr = tempArr;

		pCart->itemArr[pCart->count] = pItem;
		pCart->count++;

	}
	else {
		pItem->count += count;
	}
	return 1;
}

float	printShoppingCart(const ShoppingCart* pCart)
{
	ShoppingItem* pItem;
	float price = 0;
	for (int i = 0; i < pCart->count; i++)
	{
		pItem = pCart->itemArr[i];
		printItem(pItem);
		price += (pItem->price * pItem->count);
	}

	printf("Total bill to pay: %.2f\n", price);
	return price;
}



ShoppingItem*	getItemByBarocde(ShoppingCart* pCart, const char* barcode)
{
	for (int i = 0; i < pCart->count; i++)
	{
		if (strcmp(pCart->itemArr[i]->barcode,barcode) == 0)
			return pCart->itemArr[i];
	}
	return NULL;
}

void	freeShoppingCart(ShoppingCart* pCart)
{
	for (int i = 0; i < pCart->count; i++)
		free(pCart->itemArr[i]);

	free(pCart->itemArr);
}