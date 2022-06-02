#ifndef MAGNETOMTERTASKS_H    /* Guard against multiple inclusion */
#define MAGNETOMTERTASKS_H

#include "configuration.h"
#include "definitions.h"

typedef enum {SET_IDLE, START_SINGLE_MSMT, CHECK_DRDY, READOUT_DATA} MAG_STATE;

void vTaskGetMagMeasurement(void *pvParameters);

void i2CRunStateMachine(void);
void i2cSetIdle(MAG_STATE *State);
void i2cInitMsmnt(MAG_STATE *State);
void i2cCheckDRDY(MAG_STATE *State);
void i2cReadout(MAG_STATE *State);

#endif