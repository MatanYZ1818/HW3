#pragma once
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"

typedef enum {eNone, eName, eAmount, ePrice, eNofSortingType} eSortingType;

typedef struct
{
	char*		    name;
	Customer*	    customerArr;
	int			    customerCount;
	Product**	    productArr;
	int	            productCount;
    eSortingType    sortingType;
}SuperMarket;


int			    initSuperMarket(SuperMarket* pMarket);
void		    printSuperMarket(const SuperMarket* pMarket);
int			    addProduct(SuperMarket* pMarket);
int			    addNewProduct(SuperMarket* pMarket);
int			    addCustomer(SuperMarket* pMarket);
int			    isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int			    doShopping(SuperMarket* pMarket);
Customer*	    doPrintCart(SuperMarket* pMarket);
int			    manageShoppingCart(SuperMarket* pMarket);
Customer*	    getCustomerShopPay(SuperMarket* pMarket);
void		    fillCart(SuperMarket* pMarket, ShoppingCart* pCart);
void		    clearCart(SuperMarket* pMarket, Customer* pCustomer);

void		    printProductByType(SuperMarket* pMarket);
void		    printAllProducts(const SuperMarket* pMarket);
void		    printAllCustomers(const SuperMarket* pMarket);
int			    isBarcodeUnique(const SuperMarket* pMarket, const char* barcode);
int 		    isCustomerIdUnique(const SuperMarket* pMarket, const char* id);


int			    getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	    getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	    getProductFromUser(SuperMarket* pMarket, char* barcode);
void		    getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer*	    FindCustomerById(SuperMarket* pMarket, const char* id);

Customer*	    getCustomerWhoShop(SuperMarket* pMarket);
Product*	    getProductAndCount(SuperMarket* pMarket, int* pCount);

void		    freeMarket(SuperMarket* pMarket);
void		    freeProducts(SuperMarket* pMarket);
void		    freeCustomers(SuperMarket* pMarket);

eSortingType    typeMenu();
void            getCompareType(eSortingType sortingType, int (*compare)(const void*, const void*));
void            sortProductArray(SuperMarket* pMarket);
void            searchProduct(const SuperMarket *pMarket);
void            getProductToSearch(Product* p, eSortingType sortingType);

int             compareProductByName(const void *pProdPtrA, const void *pProdPtrB);
int             compareProductByAmount(const void *pProdPtrA, const void *pProdPtrB);
int             compareProductByPrice(const void *pProdPtrA, const void *pProdPtrB);