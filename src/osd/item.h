
#pragma once

#include "stm32f0xx.h"


#pragma anon_unions

typedef struct menu
{
    union
    {
        unsigned char uvalue;
        float fvalue;
    };
    char index;
    char item;
    struct menu *next;
    struct menu *prior;
} menu_node, *menu_list;

menu_list createMenu(char len, char item);


















