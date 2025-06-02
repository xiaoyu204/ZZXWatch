#ifndef __MY_LIST_H_
#define __MY_LIST_H_

#include "main.h"

#define Name_Max 20
#define Photo_Mode false
#define String_Mode true

typedef struct Interface
{
    uint8_t layer;
    uint8_t ID;
    uint8_t Name_length;
    char Name[Name_Max];

    struct Interface *Next_Interface;
    struct Interface *Pre_Interface;
    struct Interface *Pre_Option;
    struct Interface *Next_Option;

    void (*Function)();
} Interface_h;

void Name_Copy(char *Name, Interface_h *Option);

Interface_h *Create_HeadInterface(void (*Function)(void));
Interface_h *Create_ConnectInterface(Interface_h *Interface, char *Name, void (*Function)(void), bool Mode);
Interface_h *Create_Option(Interface_h *Option, char *Name, void (*Function)(void));
Interface_h *Create_TailInterface(Interface_h *Interface);
void Free_Option(Interface_h *option);

#endif // __MY_LIST_H_
