#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define LED_PORT GPIOA
#define LED_PIN GPIO_Pin_0
#define RCC_LED_PORT RCC_APB2Periph_GPIOA

#define LED_ON GPIO_ResetBits(LED_PORT,LED_PIN)
#define LED_OFF GPIO_SetBits(LED_PORT,LED_PIN)
void LED_Init(void);
void LED_Toggle(void);
#endif
