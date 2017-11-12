/* ----------------------- System includes -----------------------------------*/
#include <stm32f10x.h>
#include <stdio.h>
#include <stdlib.h>
/* ----------------------- FreeRTOS includes ---------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
/* ----------------------- Header includes -----------------------------------*/
#include "xprintf.h"
#include "serial.h"
/* ----------------------- Modbus includes -----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines -------------------------------------------*/
#define REG_HOLDING_START 1
#define REG_HOLDING_NREGS 15

/* ----------------------- Static variables ----------------------------------*/
static USHORT usRegHoldingStart = REG_HOLDING_START;
static USHORT usRegHoldingBuf[REG_HOLDING_NREGS] = {1, 2, 3, 4, 5};

/* ----------------------- Start implementation ------------------------------*/

eMBErrorCode
eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;

    if ((usAddress >= REG_HOLDING_START) &&
        (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegHoldingStart);
        switch (eMode)
        {
        /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while (usNRegs > 0)
            {
                *pucRegBuffer++ = (unsigned char)(usRegHoldingBuf[iRegIndex] >> 8);
                *pucRegBuffer++ = (unsigned char)(usRegHoldingBuf[iRegIndex] & 0xFF);
                iRegIndex++;
                usNRegs--;
            }
            break;

        /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while (usNRegs > 0)
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
    extern BOOL xMBPortSerialPutByte(CHAR ucByte);
void MODBUS_task(void *pvParameters)
{
    xprintf("start modbus\r");
    // eMBErrorCode errorCode = xMBPortSerialInit(1,115200,8,MB_PAR_NONE);
    eMBErrorCode errorCode = eMBInit(MB_RTU, 0x01, 1, 115200, MB_PAR_NONE);
    xprintf("ErrInit = %d\r", errorCode);
    errorCode = eMBEnable();
    xprintf("ErrEnable = %d\r", errorCode);
    // xMBPortSerialPutByte((unsigned char)'q');
    while (1)
    {
       
        // xMBPortSerialPutByte('q');
        
        errorCode = eMBPoll();
        // xprintf("ErrorPoll: %d\r",errorCode);
        // usRegHoldingBuf[0]++;
        vTaskDelay(50);
    }
}

void InitGPIO()
{
    //Init USART GPIO pins
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_Config;

    GPIO_Config.GPIO_Pin = GPIO_Pin_7;
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Config);

    GPIO_Config.GPIO_Pin = GPIO_Pin_8;
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Config);

    GPIO_Config.GPIO_Pin = GPIO_Pin_9;
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Config);
}
void Init_TIM2()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timer;
    TIM_TimeBaseStructInit(&timer);
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    timer.TIM_Prescaler = 720 - 1;
    timer.TIM_Period = 1 * 50000 - 1;
    TIM_TimeBaseInit(TIM2, &timer);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    /* TIM1 counter enable */
    TIM_Cmd(TIM2, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void InitUSART()
{
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

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}
void XPRINF_PutChar(unsigned char c)
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    // while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == SET)
        // ;
    USART_SendData(USART2, (uint8_t)c);

    // /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        ;
}
void Init_USART2()
{
    /* Enable USART2 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /* Configure USART2 Rx (PA3) as input floating */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART2 Tx (PA2) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_Cmd(USART2, ENABLE);

    xdev_out(XPRINF_PutChar);
    xprintf("UART2 Init\n");
}
void InitInterruptUsart()
{
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

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}
void InitAll()
{
    InitGPIO();
    // InitInterruptUsart();
    // InitUSART();
    // Init_TIM2();
    Init_USART2();
}
void Blinks_blue(uint16_t a)
{
    for (unsigned j = 0; j < a; j++)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_7);
        for (unsigned i = 0; i < 300000; i++)
            a++;
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        for (unsigned i = 0; i < 300000; i++)
            a--;
    }
}
void Blinks_red(uint16_t a)
{
    for (unsigned j = 0; j < a; j++)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_9);
        for (unsigned i = 0; i < 300000; i++)
            a++;
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
        for (unsigned i = 0; i < 300000; i++)
            a--;
    }
}
void Blink_blue(void *pvParameters)
{
    while (1)
    {

        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
        GPIO_SetBits(GPIOB, GPIO_Pin_7);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void Blink_green(void *pvParameter)
{
    while (1)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
        GPIO_SetBits(GPIOB, GPIO_Pin_8);
        vTaskDelay(700 / portTICK_PERIOD_MS);
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        vTaskDelay(700 / portTICK_PERIOD_MS);
    }
}
void Blink_red(void *pvParameter)
{

    while (1)
    {
        // USART_SendData(USART1, 'q');
        xprintf("asd");
        // GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        // GPIO_ResetBits(GPIOB, GPIO_Pin_8);
        GPIO_SetBits(GPIOB, GPIO_Pin_9);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

int main()
{
    uint16_t a = 0;
    a++;
    InitAll();

    Blinks_blue(1);
    if (!xTaskCreate((TaskHandle_t)Blink_red,
                     "Blink",
                     256,
                     NULL,
                     2,
                     NULL))
        configASSERT(0);
    // if (!xTaskCreate((TaskHandle_t)Blink_blue,
    //                  "Blink",
    //                  128,
    //                  NULL,
    //                  3,
    //                  NULL))
    //     configASSERT(0);
    // if (!xTaskCreate((TaskHandle_t)Blink_green,
    //                  "Blink",
    //                  128,
    //                  NULL,
    //                  4,
    //                  NULL))
    //     configASSERT(0);
    if (!xTaskCreate((TaskHandle_t)MODBUS_task,
                     "Modbus",
                     256,
                     NULL,
                     1,
                     NULL))
        configASSERT(0);

    vTaskStartScheduler();
    return 0;
}

// void USART1_IRQHandler(void)
// {
//     if (USART_GetITStatus(USART1, USART_IT_TXE) == SET){
//         Blinks_blue(1);
//     }
//     if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//     {
//         USART_ReceiveData(USART1);
//         Blinks_red(1);
//     }
// }
// void TIM2_IRQHandler(void)
// {
//     Blinks_red(1);
//     TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
// }

void vAssertCalled(const char *const file, unsigned long line)
{
    xprintf("File: %s\n", file);
    xprintf("Line: %d\n", line);
}