#include "stm32f10x.h"                  // Device header
#include "PWM.h"


/*
*此初始化MG996R舵机，PA1用于输出TIM2 OC2
*
*
*
*/
void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM2);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;   //ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;   //PSC   
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCStructure;
    TIM_OCStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStructure.TIM_Pulse = 0;  //CCR   
    TIM_OC2Init(TIM2, &TIM_OCStructure);

    TIM_Cmd(TIM2, ENABLE);
}
    

/* 0---180*/
void Servo_SetAngle(float Angle)
{
    if(Angle>180||Angle<0) return;
    TIM_SetCompare2(TIM2,Angle / 180 * 2000 + 500);
}
  
