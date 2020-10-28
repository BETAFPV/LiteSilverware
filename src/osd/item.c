#include "item.h"
#include "stdint.h"
#include "stdlib.h"

menu_list createMenu(char len, char item)
{
    char i = 0;
    menu_list pTail = NULL, p_new = NULL;
    menu_list pHead = (menu_list)malloc(sizeof(menu_node));
    if (NULL == pHead)
    {
        return 0;
    }

    pHead->index = 0;
    pHead->item = item;
    pHead->prior = pHead;
    pHead->next = pHead;
    pTail = pHead;

    for (i = 1; i < len + 1; i++)
    {
        p_new = (menu_list)malloc(sizeof(menu_node));
        if (NULL == p_new)
        {
            return 0;
        }
        p_new->index = i;
        p_new->item = item;
        p_new->prior = pTail;
        p_new->next = pHead;
        pTail->next = p_new;
        pHead->prior = p_new;
        pTail = p_new;
    }
    return pHead;
}

