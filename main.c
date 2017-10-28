#include <stm32f10x.h>
#include <stdio.h>

void InitGPIO()
{
    //Init USART GPIO pins
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_Config;
    
    GPIO_Config.GPIO_Pin = GPIO_Pin_7;
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Config);

    GPIO_Config.GPIO_Pin = GPIO_Pin_10;
    GPIO_Config.GPIO_Mode = GPIO_Mode_IN_FLOATING;
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

int main()
{
    InitAll();
    uint16_t a = 0;
    while (1)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_7);
        for (unsigned i = 0; i < 4000000; i++)
            a++;
        GPIO_ResetBits(GPIOB,GPIO_Pin_7);
        for (unsigned i = 0; i < 4000000; i++)
            a--;
    }
    return 0;
}

