#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "Delay.h"
#include "OLED.h"
#include "string.h"
#include "stdio.h"
#include "DHT.h"
#include "LED.h"


#define ESP8266_WIFI_INFO "AT+CWJAP=\"nonetheless\",\"88888888\"\r\n"
#define ESP8266_INTERNET_INFO "AT+MQTTCONN=0,\"192.168.43.222\",1883,1\r\n"
#define ESP8266_MQTT_INFO "AT+MQTTUSERCFG=0,1,\"espHome\",\"esp8266\",\"123\",0,0,\"\"\r\n"
#define ESP8266_SUB_INFO "AT+MQTTSUB=0,\"led/#\",1\r\n"   /*只需要led消息*/

/*ONENET*/
//#define ESP8266_MQTT_INFO "AT+MQTTUSERCFG=0,1,\"mqtt1\",\"EX32HOx1fD\",\"version=2018-10-31&res=products%2FEX32HOx1fD%2Fdevices%2Fmqtt1&et=2841837101&method=md5&sign=oe%2F%2B%2BAZhh6WPM5jnPIwZHA%3D%3D\",0,0,\"\"\r\n"
//#define pubtopic "$sys/EX32HOx1fD/mqtt1/thing/property/post"
//#define ESP8266_INTERNET_INFO "AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,0\r\n"
void ESP8266_Init(void);
void ESP8266_SendData(uint8_t humi,uint8_t temp);

#endif
