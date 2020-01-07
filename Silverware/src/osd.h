#ifndef __osd_h_
#define __osd_h_

#include "project.h"
#include "defines.h"

#pragma anon_unions

typedef struct menu{
    union{
        unsigned char uvalue;
        float fvalue;
    };
    char index;
    char item;
    struct menu *next;
    struct menu *prior;
}menu_node,*menu_list;

menu_list createMenu(char len,char item);

void osdMenuInit(void);

void osd_setting(void);

void getIndex(void);

uint8_t CRC8(unsigned char *data, const int8_t len);

#endif


