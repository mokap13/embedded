/********************       Module describe                ********************/

/********************       Standard libraries             ********************/

/********************       RTOS Headers                   ********************/

/********************       Header file includes           ********************/

/********************       Private types defenitions      ********************/

/********************       Private defines                ********************/

/********************       Private variable               ********************/

/********************       Private funcions               ********************/

/********************       Funcions implemetations        ********************/
void MODBUS_task(void *pvParameters)
{
    eMBInit( MB_RTU, 0x01, 1, 115200, MB_PAR_NONE );
    eMBEnable();
    while (1)
    {
        eMBPoll();
        vTaskDelay(50);
    }
}