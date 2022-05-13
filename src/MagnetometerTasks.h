#ifndef MAGNETOMTERTASKS_H    /* Guard against multiple inclusion */
#define MAGNETOMTERTASKS_H

#include "configuration.h"
#include "definitions.h"

typedef enum {SET_IDLE, START_SINGLE_MSMT, CHECK_DRDY, READOUT_DATA} SYSTEM_STATE;

void vTaskGetMagMeasurement(void *pvParameters);

void i2CRunStateMachine(void);
void i2cSetIdle(SYSTEM_STATE *State);
void i2cInitMsmnt(SYSTEM_STATE *State);
void i2cCheckDRDY(SYSTEM_STATE *State);
void i2cReadout(SYSTEM_STATE *State);

#endif