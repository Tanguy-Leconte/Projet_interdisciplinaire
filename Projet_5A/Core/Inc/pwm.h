/*
 * pwm.h
 *
 *  Created on: 2 nov. 2021
 *      Author: rapha
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "stm32l4xx_hal.h"

void GPIO_Pin_Conf(GPIO_TypeDef * GPIO,uint32_t Pin, uint32_t Mode);//PA8

void configure_PWM(TIM_TypeDef * Timer,uint32_t Channel,int Arr, int Psc);

void Set_duty_cycle(TIM_TypeDef * Timer,uint32_t Channel,float duty_cycle);

void Start_Channel(TIM_TypeDef * Timer,uint32_t Channel);

void Stop_Channel(TIM_TypeDef * Timer,uint32_t Channel);

#endif /* INC_PWM_H_ */
