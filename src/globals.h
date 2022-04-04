#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "MQTT.h"
#include "SerialCommand.h"
#include "Logging.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define MAX_RELAYS 16
#define MAX_SENSORS 4
#define MAX_BUTTONS 15


struct NetworkSettings {
    char ssid[50];
    char password[50];
    char hostname[32];
};

struct MQTTSettings {
    char broker[50];
    char port[6];
    char client_id[32];
    char topic[32];
    char user[32];
    char password[32];
};

struct Settings {
    NetworkSettings network = {
        .ssid     = "Razvan's Wi-Fi Network",
        .password = "",
        .hostname = "arduino-test"
    };
    MQTTSettings mqtt = {
        .broker    = "10.0.1.7",
        .port      = "1883",
        .client_id = "arduino",
        .topic     = "arduino",
        .user      = "",
        .password  = ""
    };
};

extern bool        relayState[MAX_RELAYS];
extern bool        sensorState[MAX_SENSORS];
extern WiFiClient  wifiClient;
extern Settings    settings;

// USE THIS TO CALCULATE : https://arduinojson.org/v5/assistant/
const size_t SETTINGS_JSON_BUFFER_SIZE = 0
    + JSON_OBJECT_SIZE(4)              // Settings        (Count of Nodes)
    + JSON_OBJECT_SIZE(6)              // MQTTSettings    (Count of Nodes)
    + JSON_OBJECT_SIZE(3)              // NetworkSettings (Count of Nodes)
    + MAX_RELAYS * JSON_OBJECT_SIZE(3)
    + MAX_BUTTONS * JSON_OBJECT_SIZE(2)
;

#endif
