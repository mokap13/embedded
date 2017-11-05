#include <stm32f10x.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "xprintf.h"
#include "serial.h"

xComPortHandle comPortHandle;

void USART_SendString(const char *str)
{
    while (*str)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
            ;
        USART_SendData(USART1, *str++);
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
}

void InitAll()
{
    InitGPIO();
    InitUSART();
}
void Blinks(uint16_t a)
{
    for (unsigned j = 0; j < a; j++)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_7);
        for (unsigned i = 0; i < 1500000; i++)
            a++;
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        for (unsigned i = 0; i < 1500000; i++)
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
    // comPortHandle = xSerialPortInitMinimal(115200, 200);
    while (1)
    {
        xprintf("Hello \r");
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
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

    Blinks(1);
    if (!xTaskCreate((TaskHandle_t)Blink_red,
                     "Blink",
                     256,
                     NULL,
                     1,
                     NULL))
        configASSERT(0);
    if (!xTaskCreate((TaskHandle_t)Blink_blue,
                     "Blink",
                     128,
                     NULL,
                     2,
                     NULL))
        configASSERT(0);
    if (!xTaskCreate((TaskHandle_t)Blink_green,
                     "Blink",
                     128,
                     NULL,
                     3,
                     NULL))
        configASSERT(0);

    vTaskStartScheduler();
    return 0;
}

void vAssertCalled(const char *const file, unsigned long line)
{
    xprintf(file);
    xprintf("\r");
    xprintf("%d", line);
    xprintf("\r");
    // while (1)
    // ;
}