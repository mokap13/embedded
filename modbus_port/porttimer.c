/*
 * FreeModbus Libary: LPC214X Port
 * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
// #include "FreeRTOS.h"

#include <stm32f10x_conf.h>
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    // // Timer0 Configuration
    // T0PR = 0;                   // Prscaler Register = 0
    // T0PC = 0;                   // Prscaler Counter = 0

    // T0TC = 0;                   // Timer Counter = 0

    // T0MR0 = ( PCLK / 20000 ) * usTim1Timerout50us;      // Interval of (50us * usTim1Timerout50us)
    // T0MCR = 3;                  // Bit 0 = 1 - Interruption on MR0
    // // Bit 1 = 1 - Reset on MR0

    // T0TCR = 0;                  // Timer Counter and Prescale Counter Disabled

    // // Configure Timer0 Interruption
    // VICVectAddr1 = ( unsigned int )prvvTIMERExpiredISR; // Timer0 Interruption - Priority 1
    // VICVectCntl1 = 0x20 | 4;
    // VICIntEnable = ( 1 << 4 );  // Enable Timer0 Interruption
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timer;
    TIM_TimeBaseStructInit(&timer);
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    timer.TIM_Prescaler = 720;
    timer.TIM_Period = 5;
    TIM_TimeBaseInit(TIM2, &timer);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    /* TIM1 counter enable */
    TIM_Cmd(TIM2, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 16;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    return TRUE;
}

void vMBPortTimersEnable()
{
    TIM_SetCounter(TIM2,0);
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM_Cmd(TIM2, ENABLE);
}

void vMBPortTimersDisable()
{
    NVIC_DisableIRQ(TIM2_IRQn);
    TIM_Cmd(TIM2, DISABLE);
}

void TIM2_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    (void)pxMBPortCBTimerExpired();
}
