/*
 * @Author: puzhiyuan puzhiyuan185489643@gmail.com
 * @Date: 2023-02-15 15:19:39
 * @LastEditors: puzhiyuan puzhiyuan185489643@gmail.com
 * @LastEditTime: 2023-02-23 21:10:55
 * @FilePath: \test01\User\main.c
 * @Description: 主函数
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "stm32f10x.h"  
#include <string.h>
#include "DHT11.h"
#include "USART.h"
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Buzzer.h"
#include "onenet.h"
#include "esp8266.h"

void Hardware_Init();
int main(void)
{
	DHT11_Data_TypeDef DHT11_Data;
	const char *topics[] = {"/mysmarthome/sub"};
	unsigned short timeCount = 0;	//发送间隔变量
	unsigned char *dataPtr = NULL;
	Hardware_Init();				//初始化外围硬件
	
	LED_ON();
	Delay_ms(500);
	LED_OFF();
	Delay_ms(1500);
	
	ESP8266_Init();					//初始化ESP8266
	while(OneNet_DevLink())			//接入OneNET
		Delay_ms(500);
	
	// 注意是有源蜂鸣器
	// Buzzer_ON();				//鸣叫提示接入成功
	// Delay_ms(250);
	// Buzzer_OFF();

	OneNet_Subscribe(topics, 1);	//订阅主题topics

	while (1)
	{
		
		if (timeCount % 40 == 0)
		{
			/* code */
		}
		
		
		if(++timeCount >= 200)		//间隔5秒上传数据
		{
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			OneNet_Publish("pcTopic", "上传数据成功");
			timeCount = 0;
			ESP8266_Clear();
		}
		if (timeCount == 40)	//间隔一秒更新一下数据
		{
			// 温湿度传感器
			if (DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS) // if( DHT11_Data.humi_deci)
			{
				OLED_ShowString(1, 1, "temp:");
				OLED_ShowString(2, 1, "humi:");
				OLED_ShowNum(1, 6, DHT11_Data.temp_int, 2);
				OLED_ShowNum(2, 6, DHT11_Data.humi_int, 2);
				OLED_ShowString(1, 9, ".");
				OLED_ShowString(2, 9, ".");
				OLED_ShowNum(1, 10, DHT11_Data.temp_deci, 2);
				OLED_ShowNum(2, 10, DHT11_Data.humi_deci, 2);
				UsartPrintf(USART1, "当前温度：%d.%d℃  湿度：%d.%d%%RH\r\n", DHT11_Data.temp_int, DHT11_Data.temp_deci, DHT11_Data.humi_int, DHT11_Data.humi_deci);
			}
			else
			{
				OLED_ShowString(3, 1, "DHT11 ERROR!");
			}
		}
		
		dataPtr = ESP8266_GetIPD(3);//参数0可能出现问题，修改为3，同时修改该函数
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		Delay_ms(10);

		
	}
}

void Hardware_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置
	LED_Init();
	OLED_Init();
	DHT11_Init();
	Buzzer_Init();
	Usart1_Init(115200);
	Usart2_Init(115200);
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
}




