#pragma once
#include "ShoppingItem.h"
#include "list.h"

typedef struct
{
	LIST*	itemList;
}ShoppingCart;


int			    initCart(ShoppingCart* pCart);
float			getTotalPrice(const ShoppingCart* pCart);
ShoppingItem*	getItemByBarocde(ShoppingCart* pCart, const char* barcode);
int				addItemToCart(ShoppingCart* pCart, const char* name, float price, int count);
float			printShoppingCart(const ShoppingCart* pCart);

void			freeShoppingCart(ShoppingCart* pCart);