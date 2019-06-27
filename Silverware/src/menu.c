#include "menu.h"
#include <stdlib.h>

Menu_List CreateDbCcLinkList(char length,char list_class)
{
		char i;
		Menu_List p_new = NULL,pTail = NULL;
		Menu_List pHead = (Menu_List)malloc(sizeof(Menu_Node));
		if(NULL == pHead)
		{
				return NULL;
		}
		pHead->menu_index = 0;
		pHead->menu_class = list_class;
		pHead->next = pHead;
		pHead->prior = pHead;
		pTail = pHead;
		
		for(i=1;i<length+1;i++)
		{
				p_new = (Menu_List)malloc(sizeof(Menu_Node));
				if(NULL == p_new)
				{
						return NULL;
				}
				
				p_new->menu_index = i;
				p_new->menu_class = list_class;
				p_new->prior = pTail;
				p_new->next = pHead;
				pTail->next = p_new;
				pHead->prior = p_new;
				pTail = p_new;
		}
		return pHead;
}
