/********************       Module describe                ********************/

/********************       Standard libraries             ********************/
#include <stdint.h>
/********************       RTOS Headers                   ********************/
#include "FreeRTOS.h"
#include "task.h"
/********************       Header file includes           ********************/
#include "mb.h"
#include "mbport.h"
/********************       Private types defenitions      ********************/

/********************       Private defines                ********************/
#define USART 3
#define DEVICE_ADDRESS 0x01

#define REG_HOLDING_START 1
#define REG_HOLDING_NREGS 15
/********************       Private variable               ********************/
static USHORT usRegHoldingStart = REG_HOLDING_START;
static USHORT usRegHoldingBuf[REG_HOLDING_NREGS] = {1, 2, 3, 4, 5};
/********************       Private funcions               ********************/

/********************       Funcions implemetations        ********************/
void MODBUS_Task(void *pvParameters)
{
    eMBErrorCode errorCode = eMBInit(MB_RTU,
                                     DEVICE_ADDRESS,
                                     USART,
                                     115200,
                                     MB_PAR_NONE);

    if(!errorCode)
        configASSERT(0);
    
    errorCode = eMBEnable();
    if(!errorCode)
        configASSERT(0);
    
    while (1)
    {
        eMBPoll();
        vTaskDelay(50);
    }
}
void MODBUS_StartTask(uint8_t priority, uint32_t stackSize)
{
    if (!xTaskCreate((TaskHandle_t)MODBUS_Task,
                     "Modbus",
                     stackSize,
                     NULL,
                     priority,
                     NULL))
        configASSERT(0);
}

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