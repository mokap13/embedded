#ifndef MODBUSTASK_H_INCLUDE
#define MODBUSTASK_H_INCLUDE

void MODBUS_Task(void *pvParameters);
void MODBUS_StartTask(uint8_t priority,uint32_t stackSize);
#endif //MODBUSTASK