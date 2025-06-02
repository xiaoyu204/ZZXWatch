#include "my_list.h"

/************************ 选项名 ***********************/
void Name_Copy(char *Name, Interface_h *Option)
{
    uint8_t x, num = 0;

    if (Name == NULL)
    {
        Option->Name_length = 0;
    }
    else
    {
        for (x = 0; x <= Name_Max; x++)
        {
            Option->Name[x] = Name[x];
            if (Option->Name[x] != ' ')
            {
                num++;
            }
            if (!Option->Name[x])
            {
                break;
            }
        }
        Option->Name_length = num - 1;
        Option->Name[x + 1] = '\0';
    }
}

/********************** 头结点创建 *********************/
Interface_h *Create_HeadInterface(void (*Function)(void))
{
    Interface_h *Interface = (Interface_h *)malloc(sizeof(Interface_h));

    Interface->layer = 1;
    Interface->ID = 1;
    Interface->Pre_Interface = NULL;
    Interface->Next_Interface = NULL;
    Interface->Pre_Option = NULL;
    Interface->Next_Option = NULL;
    Interface->Function = Function;

    return Interface;
}

/********************* 次级结点创建 ********************/
Interface_h *Create_Option(Interface_h *Option, char *Name, void (*Function)(void))
{
    Interface_h *New_Option = (Interface_h *)malloc(sizeof(Interface_h));

    Name_Copy(Name, New_Option);

    New_Option->Next_Option = NULL;
    if (!Option->Name_length) // 判断是不是第一节点
    {
        // 是第一节点
        New_Option->ID = 1;
        New_Option->Pre_Option = NULL;
        Option->Next_Option = New_Option;
        if (!Option->Function) // 判断是不是功能节点
        {
            // 是功能节点
            New_Option->Pre_Interface = Option->Pre_Interface;
        }
        else
        {
            // 不是功能节点
            New_Option->Pre_Interface = Option;
        }
    }
    else
    {
        // 不是第一节点
        // 链表尾插
        New_Option->Pre_Option = Option;
        New_Option->ID = Option->ID + 1;
        New_Option->Pre_Interface = Option->Pre_Interface;
        Option->Next_Option = New_Option;
    }

    New_Option->Function = Function;

    // 统一数据
    New_Option->layer = Option->layer;
    New_Option->Next_Interface = NULL;

    return New_Option;
}

/********************* 不同级菜单衔接 *******************/

Interface_h *Create_ConnectInterface(Interface_h *Interface, char *Name, void (*Function)(void), bool Mode)
{
    Interface_h *New_Option = (Interface_h *)malloc(sizeof(Interface_h));

    Name_Copy(Name, New_Option);

    New_Option->Pre_Interface = Interface;
    New_Option->Next_Interface = NULL;
    New_Option->Next_Option = NULL;
    New_Option->Pre_Option = NULL;
    New_Option->layer = Interface->layer + 1;
    if (!Mode) // 图片模式
    {
        New_Option->ID = 0;
    }
    else // 文字模式
    {
        New_Option->ID = 1;
    }
    Interface->Next_Interface = New_Option;

    New_Option->Function = Function;

    return New_Option;
}

/********************* 尾结点创建 *********************/
Interface_h *Create_TailInterface(Interface_h *Interface)
{
    Interface_h *New_Option = (Interface_h *)malloc(sizeof(Interface_h));

    New_Option->Pre_Interface = Interface;
    New_Option->Next_Interface = NULL;
    New_Option->Next_Option = NULL;
    New_Option->Pre_Option = NULL;
    New_Option->layer = Interface->layer + 1;
    New_Option->ID = 0;
    New_Option->Function = NULL;
    New_Option->Name_length = 2;

    Interface->Next_Interface = New_Option;

    return New_Option;
}

/*********************** 释放节点 *********************/
void Free_Option(Interface_h *option)
{
    while (option != NULL)
    {
        Interface_h *next = option->Next_Option;
        free(option);
        option = next;
    }
}

// /*********************** 释放节点 *********************/
// // 内存池的描述，使用联合体，体现穷人的智慧。就如，我一同学说的：一个字节，恨不得掰成8个字节来用。
// typedef union mem
// {
//     struct list_head list;
//     unsigned char buffer[MEM_BUFFER_SIZE];
// } mem_t;

// static union mem gmem[MEM_BUFFER_LEN];

// LIST_HEAD(mem_pool);

// // 分配内存
// void *mem_pop()
// {
//     union mem *ret = NULL;
//     psr_t psr;

//     psr = ENTER_CRITICAL();
//     if (!list_empty(&mem_pool))
//     { // 有可用的内存池
//         ret = list_first_entry(&mem_pool, union mem, list);
//         // printf("mem_pool = 0x%p  ret = 0x%p\n", &mem_pool, &ret->list);
//         list_del(&ret->list);
//     }
//     EXIT_CRITICAL(psr);
//     return ret; //->buffer;
// }

// // 回收内存
// void mem_push(void *mem)
// {
//     union mem *tmp = NULL;
//     psr_t psr;

//     tmp = (void *)mem; // container_of(mem, struct mem, buffer);
//     psr = ENTER_CRITICAL();
//     list_add(&tmp->list, &mem_pool);
//     // printf("free = 0x%p\n", &tmp->list);

//     EXIT_CRITICAL(psr);
// }

// // 初始化内存池
// void mem_pool_init()
// {
//     int i;
//     psr_t psr;
//     psr = ENTER_CRITICAL();
//     for (i = 0; i < MEM_BUFFER_LEN; i++)
//     {
//         list_add(&(gmem[i].list), &mem_pool);
//         // printf("add mem 0x%p\n", &(gmem[i].list));
//     }
//     EXIT_CRITICAL(psr);
// }
