#ifndef __MODULE_CMD_H__
#define __MODULE_CMD_H__

#include "Arduino.h"
#include "globals.h"
#include "module_mqtt.h"

void cmd_setup (void);
void cmd_loop  (void);

void cmd_RelaySet   (int relayId, bool state);
void cmd_RelayGet   (int relayId);
void cmd_RelaysGet  (void);
void cmd_SensorGet  (int sensorId);
void cmd_SensorsGet (void);

#endif