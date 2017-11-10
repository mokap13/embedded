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
 * File: $Id: portserial.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

/* ----------------------- System includes --------------------------------*/
#include <stm32f10x_conf.h>
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Static functions ---------------------------------*/

/* ----------------------- Variables ----------------------------------------*/
USART_TypeDef *USARTx;
/* ----------------------- Start implementation -----------------------------*/
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    if (xRxEnable)
    {
        USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
    }
    else
    {
        USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);
    }
    if (xTxEnable)
    {
        USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
        pxMBFrameCBTransmitterEmpty();
    }
    else
    {
        USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
    }
}

static void ComPort_InitGPIO(UCHAR ucPORT)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    switch (ucPORT)
    {
    case 1:
        /* Enable USART1 clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
        /* Configure USART1 Rx (PA10) as input floating */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure USART1 Tx (PA9) as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        break;
    case 2:
        /* Enable USART2 clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        /* Configure USART2 Rx (PA3) as input floating */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure USART2 Tx (PA2) as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        break;
    case 3:
        /* Enable USART3 clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        /* Configure USART3 Rx (PB11) as input floating */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure USART3 Tx (PB10) as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    default:
        break;
    }
}

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
    // BOOL bInitialized = TRUE;

    // USART_InitTypeDef USART_InitStructure;
    // NVIC_InitTypeDef NVIC_InitStructure;

    // switch (ucPORT)
    // {
    // case 1:
    //     USARTx = USART1;
    //     NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    //     break;
    // case 2:
    //     USARTx = USART2;
    //     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    //     break;
    // case 3:
    //     USARTx = USART3;
    //     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    //     break;
    // default:
    //     bInitialized = FALSE;
    //     break;
    // }
    // ComPort_InitGPIO(ucPORT);
    // switch (ucDataBits)
    // {
    // case 8:
    //     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    //     break;
    // case 9:
    //     USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    //     break;
    // default:
    //     bInitialized = FALSE;
    //     break;
    // }
    // USART_InitStructure.USART_BaudRate = ulBaudRate;
    // USART_InitStructure.USART_StopBits = USART_StopBits_1;
    // switch (eParity)
    // {
    // case MB_PAR_NONE:
    //     USART_InitStructure.USART_Parity = USART_Parity_No;
    //     break;
    // case MB_PAR_ODD:
    //     USART_InitStructure.USART_Parity = USART_Parity_Odd;
    //     break;
    // case MB_PAR_EVEN:
    //     USART_InitStructure.USART_Parity = USART_Parity_Even;
    //     break;
    // default:
    //     bInitialized = FALSE;
    //     break;
    // }
    // USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    // USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    // USART_Init(USARTx, &USART_InitStructure);

    // USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);

    // USART_Cmd(USARTx, ENABLE);

    // return bInitialized;

    /* Enable USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    /* Configure USART1 Rx (PA10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Tx (PA9) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // vMBPortSerialEnable(FALSE, FALSE);

    return TRUE;
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    USART_SendData(USART1, (uint16_t)ucByte);
    // while (USART_GetITStatus(USART1, USART_IT_TC) == SET)
    //     ;
    return TRUE;
}
extern void Blinks_blue(uint16_t a);
BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    // while (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    //     ;
    *pucByte = USART_ReceiveData(USART1);

    return TRUE;
}
void USARTx_Handler(void)
{
    vMBPortSetWithinException(TRUE);
    if (USART_GetITStatus(USART1, USART_IT_TXE) == !RESET)
    {
        pxMBFrameCBTransmitterEmpty();
    }
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == !RESET)
    {
        Blinks_red(1);
        pxMBFrameCBByteReceived();
    }
    vMBPortSetWithinException(FALSE);
}
void USART1_IRQHandler(void)
{
    USARTx_Handler();
}
void USART2_IRQHandler(void)
{
    USARTx_Handler();
}
void USART3_IRQHandler(void)
{
    USARTx_Handler();
}
