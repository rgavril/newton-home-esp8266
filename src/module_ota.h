#ifndef __MODULE_OTA_H__
#define __MODULE_OTA_H__

#include "Arduino.h"
#include "globals.h"

#include <ArduinoOTA.h>

void ota_setup (void);
void ota_loop  (void);

#endif