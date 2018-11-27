#ifndef __MODULE_WEB_H__
#define __MODULE_WEB_H__

#include "Arduino.h"
#include "globals.h"

#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include "module_conf.h"

void web_setup (void);
void web_loop  (void);

#endif