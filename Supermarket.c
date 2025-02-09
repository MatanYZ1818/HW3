#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "General.h"
#include "ShoppingCart.h"
#include "ClubMember.h"

static const char* typeStr[eNofProductType] = { "None", "Name", "Amount", "Price" };

int		initSuperMarket(SuperMarket* pMarket)
{
	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	pMarket->productCount = 0;
	pMarket->productArr = NULL;
    pMarket->sortingType = eNone;
	pMarket->name = getStrExactLength("Enter market name");

	if (!pMarket->name)
		return 0;

	return 1;
}

void	printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char answer;
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = NULL;

	printf("\n");
	printf("Adding new product? y/Y: ");
	scanf("%c", &answer);
	getchar();

	if (toupper(answer) == 'Y')
	{
		if (!addNewProduct(pMarket))
		{
			free(pProd);
			return 0;
		}
        if (pMarket->sortingType != eNone)
        {
            int (*compare)(const void*, const void*) = NULL;
            getCompareType(pMarket->sortingType, compare);
            qsort(pMarket->productArr, pMarket->productCount, sizeof(Product*), compare);
        }
		return 1;
	}
	else if (pMarket->productCount > 0)
	{
		printf("Do you want to increase the amount of an existing product? y/Y: ");
		scanf("%c", &answer);
		getchar();
		if (toupper(answer) == 'Y')
		{
			printAllProducts(pMarket);
			pProd = getProductFromUser(pMarket, barcode);
			if (pProd != NULL) //This barcode exist in stock
				updateProductCount(pProd);
		}
	}
	else
		return 0;
	
	return 1;
}

int		addNewProduct(SuperMarket* pMarket)
{

	Product** pTempArr = (Product**)realloc(pMarket->productArr, (pMarket->productCount + 1) * sizeof(Product*));
	if (!pTempArr)
	{
		freeProducts(pMarket);
		return 0;
	}
	pMarket->productArr = pTempArr;
	
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		free(pMarket->productArr);
		return 0;
	}

	initProductNoBarcode(pProd);

	do
	{
		generateBarcode(pProd);
	} while (!isBarcodeUnique(pMarket, pProd->barcode)); //generate new barcode until it is unique
	
	pMarket->productArr[pMarket->productCount] = pProd;
	pMarket->productCount++;
	return 1;
}

int		isBarcodeUnique(const SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (strcmp(pMarket->productArr[i]->barcode, barcode) == 0)
			return 0; // Barcode is not unique
	}
	return 1; // Barcode is unique
}

int isCustomerIdUnique(const SuperMarket* pMarket, const char* id)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].id, id) == 0)
		{
			printf("ID %s is not unique\n", id);
			return 0; // ID is not unique
		}
	}
	return 1; // ID is unique
}

int		addCustomer(SuperMarket* pMarket)
{

	Customer cust = { 0 };

	do {
		freeCustomer(&cust);

		if (!(isMember() ? initClubMember(&cust) : initCustomer(&cust)))
		{
			freeCustomer(&cust);
			return 0;
		}
	} while (!isCustomerIdUnique(pMarket, cust.id));

	if (isCustomerInMarket(pMarket, &cust))
	{
		printf("This customer already in market\n");
		free(cust.name);
		return 0;
	}

	pMarket->customerArr = (Customer*)safeRealloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
	{
		free(cust.name);
		return 0;
	}

	pMarket->customerArr[pMarket->customerCount] = cust;
	pMarket->customerCount++;
	return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
			return 1;
	}
	return 0;
}

int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;

	if (pCustomer->pCart == NULL)
	{
		pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
		if (!pCustomer->pCart)
			return 0;
		initCart(pCustomer->pCart);
	}
	fillCart(pMarket, pCustomer->pCart);
	if (pCustomer->pCart->itemList->head.next == NULL) //did not buy any thing
	{
		free(pCustomer->pCart);
		pCustomer->pCart = NULL;
	}
	printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer*	doPrintCart(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart == NULL)
	{
		printf("Customer cart is empty\n");
		return NULL;
	}
    if (pCustomer->pClubMemberObj)
        if (!addDiscount(pCustomer))
        {
            puts("Printing failed");
            return NULL;
        }
	printShoppingCart(pCustomer->pCart);
    if (pCustomer->pClubMemberObj)
        delDiscount(pCustomer);
	return pCustomer;
}

int	manageShoppingCart(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	char answer;

	if(!pCustomer)
		return 0;

	printf("Do you want to pay for the cart? y/Y, anything else to cancel shopping!\t");
	do {
		scanf("%c", &answer);
	} while (isspace(answer));

	getchar(); //clean the enter

	if (answer == 'y' || answer == 'Y')
		pCustomer->table.pay(pCustomer);
	else {
		clearCart(pMarket, pCustomer);
		cancelShopping(pCustomer);
	}
	return 1;

}

Customer*	getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->productCount == 0)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer is not listed\n");
		return NULL;
	}

	return pCustomer;
}

void	printAllProducts(const SuperMarket* pMarket)
{
	printf("There are %d products\n", pMarket->productCount);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %-20s %-15s\n", "Type", "Price", "Count In Stoke", "Expiry Date");
	printf("-------------------------------------------------------------------------------------------------\n");

    generalArrayFunction(pMarket->productArr, pMarket->productCount, sizeof(Product*), printProduct);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++)
        pMarket->customerArr[i].table.print(&pMarket->customerArr[i]);
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char searchTerm[MAX_STR_LEN];
	getsStrFixSize(searchTerm, sizeof(searchTerm), "Who is shopping? Enter customer id\n");

	Customer* pCustomer = FindCustomerById(pMarket, searchTerm);
	
	return pCustomer;
}


void fillCart(SuperMarket* pMarket, ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if(pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count = count; //item bought!!!
		}
	}
}

void clearCart(SuperMarket* pMarket, Customer* pCustomer)
{
	if (pCustomer->pCart == NULL)
		return;
	NODE *pTemp = pCustomer->pCart->itemList->head.next;
	while (pTemp != NULL)
	{
		Product* pProd = getProductByBarcode(pMarket, ((ShoppingItem*)pTemp->key)->barcode);
		if (pProd)
			pProd->count += ((ShoppingItem*)pTemp->key)->count;
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	} 
	
	if (pProd->count == 0)
	{
		printf("This product is out of stock\n");
		return NULL;
	}
	
	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
    pProd->count -= count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (pMarket->productArr[i]->type == type)
		{
			count++;
			printProduct(pMarket->productArr + i);
		}
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBarcodeCode(barcode);
	Product* pProd = getProductByBarcode(pMarket, barcode);
	if (!pProd)
	{
		printf("No such product barcode\n");
		return NULL;
	}
	return pProd;
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeProducts(pMarket);
	freeCustomers(pMarket);
}

void freeProducts(SuperMarket* pMarket)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		freeProduct(pMarket->productArr[i]);
		free(pMarket->productArr[i]);
	}
	free(pMarket->productArr);
}

void freeCustomers(SuperMarket* pMarket)
{
	for (int i = 0; i < pMarket->customerCount; i++)
        pMarket->customerArr[i].table.free(&pMarket->customerArr[i]);
	free(pMarket->customerArr);
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBarcodeCode(barcode);
		int index = getProductIndexByBarcode(pMarket, barcode);
		if (index == -1)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return i;
	}
	return -1;
}

Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return pMarket->productArr[i];
	}
	return NULL;

}


Customer* FindCustomerById(SuperMarket* pMarket, const char* id)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomerById(&pMarket->customerArr[i], id))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}

eSortingType typeMenu()
{
    int option;

    printf("\n");
    do {
        printf("Please enter one of the following types\n");
        for (int i = 1; i < eNofSortingType; i++)
            printf("%d for %s\n", i, typeStr[i]);
        scanf("%d", &option);
    } while (option < 0 || option >= eNofSortingType);

    getchar();

    return option;
}

void getCompareType(eSortingType sortingType, int (*compare)(const void*, const void*))
{
    switch (sortingType)
    {
        case eName:
            compare = compareProductByAmount;
            break;
        case eAmount:
            compare = compareProductByAmount;
            break;
        case ePrice:
            compare = compareProductByPrice;
            break;
    }
}

void sortProductArray(SuperMarket *pMarket)
{
    eSortingType type = typeMenu();
    if (pMarket->sortingType != type)
    {
        pMarket->sortingType = type;
        int (*compare)(const void*, const void*) = NULL;
        getCompareType(pMarket->sortingType, compare);
        qsort(pMarket->productArr, pMarket->productCount, sizeof(Product*), compare);
    }
}

void getProductToSearch(Product* p, eSortingType sortingType)
{
    printf("Enter the %s of the product you want to search for:\n", typeStr[sortingType]);
    switch (sortingType)
    {
        case eName:
            myGets(p->name, NAME_LENGTH + 1);
        case eAmount:
            while (True)
            {
                scanf("%d", &p->count);
                if (p->count >= 0)
                    break;
                puts("Amount should be 0 or above");
            }
            break;
        case ePrice:
            while (True)
            {
                scanf("%f", &p->price);
                if (p->price > 0)
                    break;
                puts("Price should be above 0");
            }
            break;
    }
}

void searchProduct(const SuperMarket *pMarket)
{
    if (pMarket->sortingType == eNone)
        puts("Products array isn't sorted");
    else
    {
        Product p = { 0 };
        getProductToSearch(&p, pMarket->sortingType);
        int (*compare)(const void*, const void*) = NULL;
        getCompareType(pMarket->sortingType, compare);
        Product* pProd = *(Product**) bsearch(&p , pMarket->productArr, pMarket->productCount, sizeof(Product*), compare);
        if (!pProd)
            puts("Product doesn't exist");
        else
            printProduct(pProd);
    }
}

int compareProductByName(const void *pProdPtrA,const void *pProdPtrB) {
    Product* pA = *(Product**)pProdPtrA;
    Product* pB = *(Product**)pProdPtrB;
    return strcmp(pA->name, pB->name);
}

int compareProductByAmount(const void *pProdPtrA, const void *pProdPtrB) {
    Product* pA = *(Product**)pProdPtrA;
    Product* pB = *(Product**)pProdPtrB;
    return pA->count - pB->count;
}

int compareProductByPrice(const void *pProdPtrA, const void *pProdPtrB) {
    Product* pA = *(Product**)pProdPtrA;
    Product* pB = *(Product**)pProdPtrB;
    return pA->price - pB->price;
}

