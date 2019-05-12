#include "common.h"
#include "include.h"

long quad_val_left = 0;
long quad_val_right = 0;

/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       FTM 正交解码 测试
 */
void Quad_Init(void)
{
    ftm_quad_init(FTM1); //FTM1 正交解码初始化
    ftm_quad_init(FTM2);
    // pit_init_ms(PIT0, 3);                              //初始化PIT0，定时时间为： 3ms
    // set_vector_handler(PIT0_VECTORn, PIT0_IRQHandler); //设置PIT0的中断服务函数为 PIT0_IRQHandler
    // enable_irq(PIT0_IRQn);                             //使能PIT0中断
}

/*!
 *  @brief      PIT0中断服务函数
 *  @since      v5.0
 */
// void PIT0_IRQHandler(void)
// {
//     PIT_Flag_Clear(PIT0);                //清中断标志位
//     quad_val_left += ftm_quad_get(FTM1); //获取FTM正交解码的脉冲数(负数表示反方向)
//     quad_val_right += ftm_quad_get(FTM2);
//     ftm_quad_clean(FTM1);
//     ftm_quad_clean(FTM2);
// }

//更新编码器的值
void updateQuad()
{
    quad_val_left += ftm_quad_get(FTM1); //获取FTM正交解码的脉冲数(负数表示反方向)
    quad_val_right += ftm_quad_get(FTM2);
    ftm_quad_clean(FTM1);
    ftm_quad_clean(FTM2);
}

void updateQuad()
{
    quad_val_left += ftm_quad_get(FTM1); //获取FTM正交解码的脉冲数(负数表示反方向)
    quad_val_right += ftm_quad_get(FTM2);
    ftm_quad_clean(FTM1);
    ftm_quad_clean(FTM2);
}

//110对1cm