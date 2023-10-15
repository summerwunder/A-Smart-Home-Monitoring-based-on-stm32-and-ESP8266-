#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"  
#include "LED.h"
#include "Key.h"
#include "usart.h"
#include "DHT.h"
#include "ADC.h"
#include "PWM.h"
#include "ESP8266.h"

static void DHT11_Show(void);
static void Servo_test(void);

DHT11_Data_TypeDef DHT11_Data;
uint16_t ADC1_Value;
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    OLED_Init();
    LED_Init();
    Key_Init();
    USART1_Init();
    ADC1_Init();
    DHT11_GPIO_Config();
    //PWM_Init();
    ESP8266_Init();
    OLED_ShowString(1, 1, "temp:");
    OLED_ShowString(2, 1, "humidity:");
    memset(RxData,0,200);         //将接收区清0
    while (1)
    {                   	
        if(RxFlag==1)
        {
            if(strstr(RxData,"LED_ON"))
            {
                LED_ON;
            }
            else if(strstr(RxData,"LED_OFF")) 
            {
                LED_OFF;
            }
                
            memset(RxData,0,200);         //将接收区清0
            RxFlag=0;
        }
       
        DHT11_Show();
        //ADC1_Value=ADC1_GetValue();
        //OLED_ShowNum(3,1,ADC1_Value,4);
        ESP8266_SendData(DHT11_Data.humi_int,DHT11_Data.temp_int); 
        //Servo_test();
        //printf("humi:%d temp:%d\r\n",DHT11_Data.humi_int,DHT11_Data.temp_int);
        //Delay_ms(400);        
    }
}



static void DHT11_Show(void)
{
    if(Read_DHT11(&DHT11_Data) == SUCCESS)
    {
        OLED_ShowNum(2, 10, DHT11_Data.humi_int, 2);
        OLED_ShowString(2, 12, ".");
        OLED_ShowNum(2, 13, DHT11_Data.humi_deci, 2);
        
        OLED_ShowNum(1, 6, DHT11_Data.temp_int, 2);
        OLED_ShowString(1, 8, ".");
        OLED_ShowNum(1, 9, DHT11_Data.temp_deci, 1);      
    }
}
/*
static void Servo_test(void)
{
    Servo_SetAngle(0);
    Delay_s(1);
    Servo_SetAngle(180);
    Delay_s(1);
}
*/
