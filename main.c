#include <stm32f10x.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "serial.h"

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

void InitAll()
{
    InitGPIO();
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
    xComPortHandle comPortHandle = xSerialPortInitMinimal(115200,20);
    while (1)
    {
        vSerialPutString(comPortHandle,(const signed char * const )"Abra \r",(unsigned short)0);
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
                2,
                NULL);
    xTaskCreate((TaskHandle_t)Blink_green,
                "Blink",
                128,
                NULL,
                3,
                NULL);
    xTaskCreate((TaskHandle_t)Blink_red,
                "Blink",
                256,
                NULL,
                1,
                NULL);
    vTaskStartScheduler();
    return 0;
}
