cmake_minimum_required(VERSION 3.30)
project(HW3 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(HW3
    Customer.c
    Customer.h
    Date.c
    Date.h
    General.c
    General.h
    main.c
    main.h
    Product.c
    Product.h
    ShoppingCart.c
    ShoppingCart.h
    ShoppingItem.c
    ShoppingItem.h
    Supermarket.c
    Supermarket.h
    list.c
    list.h
    def.h)
