#ifndef GNSSTASKS_H    /* Guard against multiple inclusion */
#define GNSSTASKS_H

#include "configuration.h"
#include "definitions.h"

void vTaskGetNEOM8Mbyte( void *pvParameters );
void vTaskProcessNEOM8Mstring( void *pvParameters );

#endif
