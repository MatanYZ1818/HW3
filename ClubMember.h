//
// Created by sagib on 22/01/2025.
//

#pragma once

#include "Customer.h"

typedef struct
{
    int totalMonth;
} ClubMember;

int initClubMember(Customer* pCustomer);

void printClubMember(const Customer* pCustomer);

void clubMemberPay(Customer* pCustomer);

int addDiscount(Customer* pCustomer);

void delDiscount(Customer* pCustomer);
