#pragma once
#include "ShoppingItem.h"
#include "list.h"

typedef struct
{
	LIST*	itemList;
}ShoppingCart;


int				initCart(ShoppingCart* pCart);
float			getTotalPrice(const ShoppingCart* pCart);
ShoppingItem*	getItemByBarocde(ShoppingCart* pCart, const char* barcode);
int				addItemToCart(ShoppingCart* pCart, const char* barcode, float price, int count);
float			printShoppingCart(const ShoppingCart* pCart);
int				freeShoppingCart(ShoppingCart* pCart);