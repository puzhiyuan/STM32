#include "stm32f10x.h"
#include "Delay.h"
#include "USART.h"
#include "OLED.h"
#include "DHT11.h"

int main(void)
{
	DHT11_Data_TypeDef DHT11_Data;
	OLED_Init();
	USART_Config();
	DHT11_Init();
	OLED_ShowString(3, 1, "flag:");
	OLED_ShowNum(3, 6, DHT11_Read_TempAndHumidity(&DHT11_Data), 1);
	while (1)
	{
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

		}
		else
		{
			OLED_ShowString(3, 1, "DHT11 ERROR!");
		}
		Delay_ms(2000);
	}
}
