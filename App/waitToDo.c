#include "common.h"
#include "include.h"

void waitToDo()
{
    /*执行命令行*/
    if (uart4_to_do_flag)
    {
        uart4_to_do_flag = 0;
        analysisCommand();
    }
    /*更新UI*/
    if (1 == key_on && (p_current_state == &wait_middle ||
                        p_current_state == &normal_page))
    {
        updateUI();
    }
    key_on = 0;
}