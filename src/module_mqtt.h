#ifndef __MODULE_MQTT_H__
#define __MODULE_MQTT_H__

#include "Arduino.h"
#include "globals.h"
#include "module_cmd.h"

void mqtt_setup (void);
void mqtt_loop  (void);

void mqtt_statRelay(uint8_t relayId, bool state);
void mqtt_statSensor(uint8_t sensorId, bool state);
void mqtt_statButton(uint8_t buttonId, uint8_t eventId);

#endif