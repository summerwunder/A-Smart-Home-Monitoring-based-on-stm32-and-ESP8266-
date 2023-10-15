#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define KEY_PORT GPIOA
#define KEY_PIN GPIO_Pin_5
#define RCC_KEY_PORT RCC_APB2Periph_GPIOA
void Key_Init(void);
uint8_t Key_GetValue(void);
#endif

