
#include "Key.h"

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_KEY_PORT,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=KEY_PIN;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(KEY_PORT,&GPIO_InitStructure);           
}


/*
    ��ȡ����ֵ
    ����ֵ��1��ʾ����
            0��ʾδ����
*/
uint8_t Key_GetValue(void)
{
    uint8_t Value;
    Value=GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN);
    if(Value==0x00)
    {
        Delay_ms(20);
        while(GPIO_ReadInputDataBit(KEY_PORT,KEY_PIN)==0x00);
        Delay_ms(20);   
        return 1;
    }
    return 0;
}
