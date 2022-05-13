#ifndef GNSSTASKS_H    /* Guard against multiple inclusion */
#define GNSSTASKS_H

#include "configuration.h"
#include "definitions.h"

void vTaskGetNEOM8Mdata( void *pvParameters );
void vTaskSendNEOM8Mdata( void *pvParameters );

#endif
