//
// Created by sagib on 22/01/2025.
//

#pragma once

struct _Customer;

typedef struct
{
    void (*pay)(struct _Customer*);
    void (*print)(const struct _Customer*);
    void (*free)(struct _Customer*);
} CustomerVTable;

