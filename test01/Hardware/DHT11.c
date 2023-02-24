#include "stm32f10x.h" // Device header
#include "DHT11.h"
#include "Delay.h"

#define DHT11_Dout_SCK_APBxClock_FUN RCC_APB2PeriphClockCmd
#define DHT11_Dout_GPIO_CLK RCC_APB2Periph_GPIOE
#define DHT11_Dout_GPIO_PORT GPIOA
#define DHT11_Dout_GPIO_PIN GPIO_Pin_4

#define DHT11_Dout_0 GPIO_ResetBits(DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN)
#define DHT11_Dout_1 GPIO_SetBits(DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN)
#define DHT11_Dout_IN() GPIO_ReadInputDataBit(DHT11_Dout_GPIO_PORT, DHT11_Dout_GPIO_PIN)
static void DHT11_GPIO_Config(void);

void DHT11_Init(void)
{
	DHT11_GPIO_Config();
	DHT11_Dout_1;
}

static void DHT11_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DHT11_Dout_SCK_APBxClock_FUN(DHT11_Dout_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);
}

static void DHT11_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);
}

static void DHT11_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);
}

static uint8_t DHT11_ReadByte(void)
{
	uint8_t i, temp = 0;
	for (i = 0; i < 8; i++)
	{
		while (DHT11_Dout_IN() == Bit_RESET)
			;
		Delay_us(40);
		if (DHT11_Dout_IN() == Bit_SET)
		{
			while (DHT11_Dout_IN() == Bit_SET)
				;
			temp |= (uint8_t)(0x01 << (7 - i));
		}
		else
		{
			temp &= (uint8_t) ~(0x01 << (7 - i));
		}
	}
	return temp;
}

uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Mode_Out_PP();
	DHT11_Dout_0;
	Delay_ms(18);
	DHT11_Dout_1;
	Delay_us(30);
	DHT11_Mode_IPU();
	if (DHT11_Dout_IN() == Bit_RESET)
	{
		while (DHT11_Dout_IN() == Bit_RESET)
			;
		while (DHT11_Dout_IN() == Bit_SET)
			;
		DHT11_Data->humi_int = DHT11_ReadByte();
		DHT11_Data->humi_deci = DHT11_ReadByte();
		DHT11_Data->temp_int = DHT11_ReadByte();
		DHT11_Data->temp_deci = DHT11_ReadByte();
		DHT11_Data->check_sum = DHT11_ReadByte();
		DHT11_Mode_Out_PP();
		DHT11_Dout_1;
		if (DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int + DHT11_Data->temp_deci)
			return SUCCESS;
		else
			return ERROR;
	}
	else
	{
		return ERROR;
	}
}

/*
#define DHT11_RCC RCC_APB2Periph_GPIOA
#define DHT11_PPOR GPIOA
#define DHT11_PIN GPIO_Pin_10

uint8_t data[5] = {0x00, 0x00, 0x00, 0x11};
uint8_t sum = 0;
// stm32输出信号
void DHT11_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PPOR, &GPIO_InitStruct);
}

// DHT11输入信号
void DHT11_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PPOR, &GPIO_InitStruct);
}

uint8_t DHT11_Read_Byte(void)
{
	uint8_t i;
	uint8_t temp;
	uint8_t data = 0;
	uint8_t retry = 0;
	for (i = 0; i < 8; i++)
	{
		while (!GPIO_ReadInputDataBit(DHT11_PPOR, DHT11_PIN) && retry < 100)
		{
			Delay_us(1);
			retry++;
		}
		if (retry >= 100)
		{
			return 0;
		}
		else
		{
			retry = 0;
		}
		Delay_us(30);
		temp = 0;
		if (GPIO_ReadInputDataBit(DHT11_PPOR, DHT11_PIN) == 1)
		{
			temp = 1;
		}

		while (GPIO_ReadInputDataBit(DHT11_PPOR, DHT11_PIN) && retry < 100)
		{
			Delay_us(1);
			retry++;
		}
		retry = 0;
		data <<= 1;
		data |= temp;
	}
	return data;
}

uint8_t DHT11_Read_Data(void)
{
	uint8_t i;
	uint8_t retry;
	DHT11_Output();
	GPIO_ResetBits(DHT11_PPOR, DHT11_PIN);
	Delay_us(18);

	GPIO_SetBits(DHT11_PPOR, DHT11_PIN);
	Delay_us(40);

	DHT11_Input();
	Delay_us(20);
	if (!GPIO_ReadInputDataBit(DHT11_PPOR, DHT11_PIN))
	{
		while (!GPIO_ReadInputDataBit(DHT11_PPOR, DHT11_PIN) && retry < 100)
		{
			Delay_us(1);
			retry++;
		}
		retry = 0;
		while (GPIO_ReadInputDataBit(DHT11_PPOR, DHT11_PIN) && retry < 100)
		{
			Delay_us(1);
			retry++;
		}
		retry = 0;

		for (i = 0; i < 5; i++)
		{
			data[i] = DHT11_Read_Byte();
		}
		Delay_us(50);
	}
	sum = data[0] + data[1] + data[2] + data[3];
	if (sum == data[4])
	{
		return 1;
	}else{
		return 0;
	}

}
*/
