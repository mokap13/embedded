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
#include "modbusTask.h"


/* ----------------------- Defines -------------------------------------------*/

/* ----------------------- Static variables ----------------------------------*/

/* ----------------------- Start implementation ------------------------------*/

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

void InitAll()
{
    InitGPIO();
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
                     1,
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
                     384,
                     NULL,
                     2,
                     NULL))
        configASSERT(0);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    vTaskStartScheduler();
    return 0;
}

void vAssertCalled(const char *const file, unsigned long line)
{
    xprintf("File: %s\n", file);
    xprintf("Line: %d\n", line);
}