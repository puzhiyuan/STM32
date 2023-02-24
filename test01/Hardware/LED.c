/*
 * @Author: puzhiyuan puzhiyuan185489643@gmail.com
 * @Date: 2023-02-15 15:19:38
 * @LastEditors: puzhiyuan puzhiyuan185489643@gmail.com
 * @LastEditTime: 2023-02-23 14:41:15
 * @FilePath: \test01\Hardware\LED.c
 * @Description: LED驱动
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "stm32f10x.h"                  // Device header


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


void LED_ON(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void LED_OFF(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
