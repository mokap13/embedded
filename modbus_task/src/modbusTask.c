/********************       Module describe                ********************/

/********************       Standard libraries             ********************/

/********************       RTOS Headers                   ********************/

/********************       Header file includes           ********************/
#include "mb.h"
#include "mbport.h"
/********************       Private types defenitions      ********************/

/********************       Private defines                ********************/
#define REG_HOLDING_START 1
#define REG_HOLDING_NREGS 15
/********************       Private variable               ********************/
static USHORT usRegHoldingStart = REG_HOLDING_START;
static USHORT usRegHoldingBuf[REG_HOLDING_NREGS] = {1, 2, 3, 4, 5};
/********************       Private funcions               ********************/

/********************       Funcions implemetations        ********************/
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