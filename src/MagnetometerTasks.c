#include "MagnetometerTasks.h"

volatile bool i2cTransferComplete = false;
//volatile uint8_t i2CStep = 1;

MAG_STATE i2c1State;

void (* const pStateFunction[])(MAG_STATE *State) =
{
    &i2cSetIdle,
    &i2cInitMsmnt,
    &i2cCheckDRDY,
    &i2cReadout
};

uint16_t Address = 0x0C;
uint8_t cmdIdle[] = {0x10, 0x00};
uint8_t cmdSingle[] = {0x10, 0x01};
uint8_t resultCheckDRDY[1];
uint8_t resultMeasurement[12];

static void i2c1EventHandler(uintptr_t context)
{
    i2cTransferComplete = true;
}

//void vTaskGetMagMeasurement(void *pvParameters)
//{
//    uint16_t Address = 0x0C;
//    
//    uint8_t cmdIdle[] = {0x10, 0x00};
//    uint8_t cmdSingle[] = {0x10, 0x01};
//    uint8_t resultCheckDRDY[1];
//    uint8_t resultMeasurement[12];
//    
//    I2C1_CallbackRegister(i2c1EventHandler, 0);
//    
//    while(1)
//    {    
//        /* Temporary code for testing i2c bus and functions*/
//        
//        switch (i2CStep)
//        {
//            case 1:
//                /* Set Idle mode */
//                I2C1_Write(Address, cmdIdle, 2);
//                while(i2cTransferComplete == false); //wait here until complete
//                i2cTransferComplete = false;
//                i2CStep = 2;
//                HB_LED_Toggle();
//                break;
//                
//            case 2:
//                /* Initiate single measurement */
//                I2C1_Write(Address, cmdSingle, 2);
//                while(i2cTransferComplete == false); //wait here until complete
//                i2cTransferComplete = false;
//                i2CStep = 3;
//                break;
//            
//            case 3:
//                /* check DRDY */
//                I2C1_Read(Address, resultCheckDRDY , 1);
//                while(i2cTransferComplete == false); //wait here until complete
//                i2cTransferComplete = false;
//                i2CStep = 4;
//                GPS_LED_Toggle();
//                break;
//                
//            case 4:
//                /* Readout Data */
//                I2C1_Read(Address, resultMeasurement , 12);
//                while(i2cTransferComplete == false); //wait here until complete
//                i2cTransferComplete = false;
//                i2CStep = 1;
//                break;
//                
//            default:
//                break;
//        }
//       
//        /* Delay 1 second */
//        vTaskDelay(1000);
//   }
//    
//}
////////////////////////////////////////////////////////////////////////////////
//Implement Task as State machine with function pointers

void vTaskGetMagMeasurement(void *pvParameters)
{
    
    I2C1_CallbackRegister(i2c1EventHandler, 0);
    
    while(1)
    {
        i2CRunStateMachine();
        vTaskDelay(1000);
    } 
}

void i2cSetIdle(MAG_STATE *State)
{
    I2C1_Write(Address, cmdIdle, 2);
    while(i2cTransferComplete == false); //wait here until complete
    i2cTransferComplete = false;
    (*State)++;
}

void i2cInitMsmnt(MAG_STATE *State)
{
    I2C1_Write(Address, cmdSingle, 2);
    while(i2cTransferComplete == false); //wait here until complete
    i2cTransferComplete = false;
    (*State)++;
}

void i2cCheckDRDY(MAG_STATE *State)
{
    I2C1_Read(Address, resultCheckDRDY , 1);
    while(i2cTransferComplete == false); //wait here until complete
    i2cTransferComplete = false;
    (*State)++;
}

void i2cReadout(MAG_STATE *State)
{
    I2C1_Read(Address, resultMeasurement , 12);
    while(i2cTransferComplete == false); //wait here until complete
    i2cTransferComplete = false;
    (*State) = (*State)-3;
}

void i2CRunStateMachine(void)
{
    (*pStateFunction[i2c1State])(&i2c1State);
}
