#include "Softwaretimer.h"
#include "stm8l15x_tim1.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_clk.h"

void SOFTWARETIMER_Init(void)
{
    /* Enable TIM1 CLK */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
    /* Time base configuration */
    TIM1_TimeBaseInit(16, TIM1_CounterMode_Up, 999, 0);
    /* Clear TIM1 update flag */
    TIM1_ClearFlag(TIM1_FLAG_Update);
    /* Enable update interrupt */
    TIM1_ITConfig(TIM1_IT_Update, ENABLE);
    /* Enable TIM1 */
    TIM1_Cmd(ENABLE);
}

void Timerwakeuptemper_Init(void)
{
    /* Enable TIM2 CLK */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    /* Time base configuration */
    TIM2_TimeBaseInit(TIM2_Prescaler_1, TIM2_CounterMode_Up, 11400);
    /* Clear TIM2 update flag */
    TIM2_ClearFlag(TIM2_FLAG_Update);
    /* Enable update interrupt */
    TIM2_ITConfig(TIM2_IT_Update, ENABLE);
    /* Enable TIM2 */
    TIM2_Cmd(ENABLE);
}
