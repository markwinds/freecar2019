#include "common.h"
#include "include.h"

char uart4_buff[1024] = {0};
int uart4_buff_size = 0;
int uart4_to_do_flag = 0;

// //处理buff函数
// void buffHandler()
// {
//     for (int i = 0; i < 101; i++)
//     {
//         DELAY_MS(20);
//         printf("%d,%d ", i, steerTable[i]);
//     }
// }

//printf函数输出串口收到数据的中断处理函数
void uart4_handler(void)
{
    char ch;
    UARTn_e uratn = UART4;
    if (UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK) //接收数据寄存器满
    {
        uart_getchar(UART4, &ch); //无限等待接受1个字节
        if (ch == '\n')
        {
            //在这里加入缓存处理
            uart4_to_do_flag = 1;
            uart4_buff[uart4_buff_size] = '\0';
            uart4_buff_size = 0;
        }
        else
            uart4_buff[uart4_buff_size++] = ch;
    }
}

//初始化串口中断
void initUart()
{
    uart_init(UART4, 115200);
    set_vector_handler(UART4_RX_TX_VECTORn, uart4_handler);
    uart_rx_irq_en(UART4);
}