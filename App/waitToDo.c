#include "common.h"
#include "include.h"

void waitToDo()
{
    if (uart4_to_do_flag)
    {
        uart4_to_do_flag = 0;
        buffHandler();
    }
}