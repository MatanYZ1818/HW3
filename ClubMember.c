//
// Created by sagib on 22/01/2025.
//

#include "ClubMember.h"

#include <stdlib.h>
#include <stdio.h>

int initClubMember(Customer* pCust)
{
    if (!initCustomer(pCust))
    {
        return False;
    }
    ClubMember* pClubMember = (ClubMember*)malloc(sizeof(ClubMember));
    if (!pClubMember)
    {
        return False;
    }

    int months;
    do
    {
        puts("Enter how many months the customer was a club member (can't be 0 or below):");
        scanf("%d", &months);
    } while (months <= 0);
    pClubMember->totalMonth = months;
    pCust->pClubMemberObj = pClubMember;

    pCust->table.pay = clubMemberPay;
    pCust->table.print = printClubMember;
    return True;
}

void printClubMember(const Customer *pCust) {
    printCustomer(pCust);
    printf("%s is a club member for %d months\n", pCust->name, ((ClubMember*)pCust->pClubMemberObj)->totalMonth);
}

void clubMemberPay(Customer* pCust)
{
    if (!addDiscount(pCust))
    {
        puts("Payment failed");
        return;
    }
    customerPay(pCust);
}

int addDiscount(Customer* pCust)
{
    float discount;
    ClubMember* pClubMember = pCust->pClubMemberObj;
    if (pClubMember->totalMonth < 24)
        discount = 0.1 * pClubMember->totalMonth;
    else
    {
        if(pClubMember->totalMonth<70)
            discount = 2.5 + 0.5 * pClubMember->totalMonth;
        else
            discount = 7.5;
    }
    return addItemToCart(pCust->pCart,"ZZ99999", (float)(-(getTotalPrice(pCust->pCart) * discount * 0.01)),1);
}

void delDiscount(Customer* customer)
{
    NODE *pTemp = &customer->pCart->itemList->head;
    while (pTemp->next != NULL && ((ShoppingItem*)pTemp->next->key)->barcode[0] != 'Z') pTemp = pTemp->next;
    L_delete(pTemp);
}

void freeClubMember(Customer* pCust)
{
    freeCustomer(pCust);
    free(pCust->pClubMemberObj);
}



