#ifndef __ADC_H
#define __ADC_H

#define ADC_CHANNEL ADC_Channel_6
#define ADC_GPIO_PIN GPIO_Pin_6
#define ADC_GPIO_PORT GPIOA
#define ADC_RCC_PORT RCC_APB2Periph_ADC1
#define ADC_GPIO_RCC_PORT RCC_APB2Periph_GPIOA

void ADC1_Init(void);
uint16_t ADC1_GetValue(void);
#endif
