/*******************************************************************************
 System Tasks File

  File Name:
    tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled tasks.

  Description:
    This file contains source code necessary to maintain system's polled tasks.
    It implements the "SYS_Tasks" function that calls the individual "Tasks"
    functions for all polled MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"


// *****************************************************************************
// *****************************************************************************
// Section: RTOS "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

volatile bool uart1RxReceived = false;
volatile bool i2cTransferComplete = false;
volatile uint8_t  i2CStep = 1;

QueueHandle_t uart1Queue;

static void uart1ReadEventHandler (uintptr_t context)
{
    uart1RxReceived = true;
}
    
void vTaskGetNEOM8Mdata( void *pvParameters )
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

static void i2c1EventHandler(uintptr_t context)
{
    i2cTransferComplete = true;
}

void vTaskGetMagMeasurement(void *pvParameters)
{
    uint16_t Address = 0x0C;
    
    uint8_t cmdIdle[] = {0x10, 0x00};
    uint8_t cmdSingle[] = {0x10, 0x01};
    uint8_t resultCheckDRDY[1];
    uint8_t resultMeasurement[12];
    
    I2C1_CallbackRegister(i2c1EventHandler, 0);
    
    while(1)
    {    
        /* Temporary code for testing i2c bus and functions*/
        
        switch (i2CStep)
        {
            case 1:
                /* Set Idle mode */
                I2C1_Write(Address, cmdIdle, 2);
                while(i2cTransferComplete == false); //wait here until complete
                i2cTransferComplete = false;
                i2CStep = 2;
                HB_LED_Toggle();
                break;
                
            case 2:
                /* Initiate single measurement */
                I2C1_Write(Address, cmdSingle, 2);
                while(i2cTransferComplete == false); //wait here until complete
                i2cTransferComplete = false;
                i2CStep = 3;
                break;
            
            case 3:
                /* check DRDY */
                I2C1_Read(Address, resultCheckDRDY , 1);
                while(i2cTransferComplete == false); //wait here until complete
                i2cTransferComplete = false;
                i2CStep = 4;
                GPS_LED_Toggle();
                break;
                
            case 4:
                /* Readout Data */
                I2C1_Read(Address, resultMeasurement , 12);
                while(i2cTransferComplete == false); //wait here until complete
                i2cTransferComplete = false;
                i2CStep = 1;
                break;
                
            default:
                break;
        }
       
        /* Delay 1 second */
        vTaskDelay(1000);
    }
    
}

void vTask2( void *pvParameters )
{
    uint8_t dataByte;
    
    while(1)
    {
        xQueueReceive( uart1Queue, &dataByte, 200 );
        UART2_Write( &dataByte, 1);
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/
void SYS_Tasks ( void )
{
    /* Maintain system services */
    /* Maintain Device Drivers */
    /* Maintain Middleware & Other Libraries */
    /* Start RTOS Scheduler. */
    
     /**********************************************************************
     * Create all Threads for APP Tasks before starting FreeRTOS Scheduler *
     ***********************************************************************/
   uart1Queue = xQueueCreate( 1, sizeof( uint8_t ) );
    
   if( uart1Queue != NULL )
   {
        xTaskCreate( vTaskGetNEOM8Mdata, "GetNEOM8Mdata", 1000, NULL, 1, NULL );
        xTaskCreate( vTask2, "Task 2", 1000, NULL, 1, NULL );
        xTaskCreate(vTaskGetMagMeasurement, "GetMagMeasurement", 1000, NULL, 1, NULL);
    
        vTaskStartScheduler(); /* This function never returns. */
   }
   else
   {
       //The queue could not be created
   }
}

/*******************************************************************************
 End of File
 */

