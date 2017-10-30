#include <stm32f10x.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

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
        vTaskDelay(100 / portTICK_PERIOD_MS);
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        vTaskDelay(100 / portTICK_PERIOD_MS);
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

    xTaskCreate((TaskHandle_t)Blink_blue,
                "Blink",
                128,
                NULL,
                configMAX_PRIORITIES - 2,
                NULL);
    xTaskCreate((TaskHandle_t)Blink_green,
                "Blink",
                128,
                NULL,
                configMAX_PRIORITIES - 1,
                NULL);
    // xTaskCreate((TaskHandle_t)Blink_red,
    //             "Blink",
    //             128,
    //             NULL,
    //             configMAX_PRIORITIES - 3,
    //             NULL);
    vTaskStartScheduler();
    return 0;
}
