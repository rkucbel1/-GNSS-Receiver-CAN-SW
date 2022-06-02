#include "GNSStasks.h"

extern QueueHandle_t uart1Queue;

volatile bool uart1RxReceived = false;

static void uart1ReadEventHandler (uintptr_t context)
{
    uart1RxReceived = true;
}
    
void vTaskGetNEOM8Mbyte( void *pvParameters )
{
    uint8_t readByte;
     
    UART1_ReadCallbackRegister(uart1ReadEventHandler, 0);
   
    while(1)
    {
        UART1_Read(&readByte, 1);
        
        if(uart1RxReceived == true)
        {
            uart1RxReceived = false;
            xQueueOverwrite( uart1Queue, &readByte );
        }
    }
}

void vTaskProcessNEOM8Mstring( void *pvParameters )
{
    uint8_t dataByte;
    
    while(1)
    {
        xQueueReceive( uart1Queue, &dataByte, 200 );
        UART2_Write( &dataByte, 1);
    }
}