#ifndef __MODULE_CONF_H__
#define __MODULE_CONF_H__

#include "globals.h"
#include <FS.h>
#include <ArduinoJson.h>

void conf_FactoryReset(void);
void conf_SaveSettings(void);
void conf_LoadSettings(void);
void conf_SettingsFromJsonObject(JsonObject &jsonSettings);

#endif