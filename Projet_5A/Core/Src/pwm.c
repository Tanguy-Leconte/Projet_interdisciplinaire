/*
 * pwm.c
 *
 *  Created on: 2 nov. 2021
 *      Author: rapha
 */
#include "pwm.h"

void GPIO_Pin_Conf(GPIO_TypeDef * GPIO,uint32_t Pin, uint32_t Mode)//PA8
{

		// Validation horloge locale
		if (GPIO==GPIOA)
			LL_AHB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		else if (GPIO==GPIOB)
			LL_AHB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
		else if (GPIO==GPIOC)
			LL_AHB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
		else if (GPIO==GPIOD)
			LL_AHB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);

	  LL_GPIO_SetPinMode(GPIO,Pin,Mode);

}

void configure_PWM(TIM_TypeDef * Timer,uint32_t Channel,int Arr, int Psc)
{
		LL_TIM_InitTypeDef My_LL_Tim_Init_Struct;

		// Validation horloge locale
		if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
		else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
		else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
		else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);


		// initialisation valeur autoreload, prescaler
		LL_TIM_SetAutoReload(TIM_TypeDef * Timer,Arr);
		LL_TIM_SetPrescaler(TIM_TypeDef * Timer,Psc);

		LL_TIM_OC_ConfigOutput(Timer,Channel,LL_TIM_OCPOLARITY_HIGH);
		LL_TIM_OC_SetMode(Timer,Channel,LL_TIM_OCMODE_PWM1);



	  LL_TIM_Init(Timer,&My_LL_Tim_Init_Struct);
}

void Set_duty_cycle(TIM_TypeDef * Timer,uint32_t Channel,float duty_cycle){
	if (Channel==LL_TIM_CHANNEL_CH1)
		LL_TIM_OC_SetCompareCH1 (Timer, duty_cycle);
	else if (Channel==LL_TIM_CHANNEL_CH2)
		LL_TIM_OC_SetCompareCH2 (Timer, duty_cycle);
	else if (Channel==LL_TIM_CHANNEL_CH3)
		LL_TIM_OC_SetCompareCH3 (Timer, duty_cycle);
	else if (Channel==LL_TIM_CHANNEL_CH4)
		LL_TIM_OC_SetCompareCH4 (Timer, duty_cycle);
}

void Start_Channel(TIM_TypeDef * Timer,uint32_t Channel)
{
	LL_TIM_CC_EnableChannel(Timer,Channel);
}

void Stop_Channel(TIM_TypeDef * Timer,uint32_t Channel)
{
	LL_TIM_CC_DisableChannel(Timer,Channel);
}



