#include "ESP8266.h"
void ESP8266_Init(void)
{

    printf("AT\r\n");//第一步		AT
    Delay_ms(1000);	//延迟  
    /*if(strstr((const char*)RxData, "OK") != NULL)
    {
       OLED_ShowString(3,1,"START right");
    }*/
    
    printf("AT+RST\r\n");
    Delay_ms(1000);
    
    printf("AT+CWMODE=1\r\n");
    Delay_ms(1000);
    
    printf("AT+CIPMUX=1\r\n");
    Delay_ms(1000);
    
    printf(ESP8266_WIFI_INFO);
    Delay_ms(2000);
        
    USART_SendString(ESP8266_MQTT_INFO);
    Delay_ms(1000);
    
    printf(ESP8266_INTERNET_INFO);
    Delay_ms(3000);
    
    printf(ESP8266_SUB_INFO);
    Delay_ms(1000);
    
    
    /*ONENET*/
    //printf("AT+MQTTSUB=0,\"$sys/EX32HOx1fD/mqtt1/thing/property/post/reply\",1\r\n");
    //Delay_ms(3000);
    //printf("AT+MQTTSUB=0,\"$sys/EX32HOx1fD/$mqtt1/user/get\",1\r\n");
    //Delay_ms(3000);  
    
    
}
//AT+MQTTPUB=0,"esp/1","{\"temp\":30\,\"humi\":10}",1,0
void ESP8266_SendData(uint8_t humi,uint8_t temp)
{
    char pubtopic[100]="esp/1";
    char cmdBuf[512];    
    sprintf(cmdBuf, "AT+MQTTPUB=0,\"%s\",\"{\\\"humi\\\":%d\\,\\\"temp\\\":%d}\",1,0\r\n",pubtopic,humi,temp);//发送命令
    printf("%s",cmdBuf);
    Delay_ms(20);
    memset(cmdBuf,0,sizeof(cmdBuf));   
}




