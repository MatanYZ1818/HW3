//
// Created by sagib on 22/01/2025.
//

#include "ClubMember.h"

#include <stdlib.h>
#include <stdio.h>

int initClubMember(Customer* pCustomer)
{
    ClubMember* pClubMember = (ClubMember*)malloc(sizeof(ClubMember));
    if (!pClubMember)
    {
        freeCustomer(pCustomer);
        return False;
    }

    int months;
    do
    {
        puts("Enter duration of club membership in months (can't be 0 or below):");
        scanf("%d", &months);
    } while (months <= 0);
    pClubMember->totalMonth = months;
    pCustomer->pDeprivedObj = pClubMember;
    return True;
}
