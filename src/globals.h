#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "MQTT.h"
#include "SerialCommand.h"
#include "Logging.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define MAX_RELAYS 16
#define MAX_SENSORS 2
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

struct ButtonSettings {
    char name[32];
    int  toggle_relay_id;
};

struct RelaySettings {
    char  name[32];
    bool  default_state;
    bool  is_persistent;
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
    ButtonSettings button[MAX_BUTTONS] = {
        {"Intrerupator Baie Oaspeti I"   ,  5}, //  0
        {"Intrerupator Hol Dormitor"     ,  3}, //  1
        {"Intrerupator Intrare"          ,  8}, //  2
        {"Intrerupator Perete Dining IV" ,  1}, //  3
        {"Intretupator Baie Oaspeti III" , 11}, //  4
        {"Intrerupator Balcon"           ,  0}, //  5
        {"Intrerupator Baie Dormitor I"  , 10}, //  6
        {"Intrerupator Birou"            ,  4}, //  7
        {"Intrerupator Dressing"         ,  7}, //  8
        {"Intrerupator Perete Dining II" ,  6}, //  9
        {"Intrerupator Baie Dormitor II" ,  9}, // 10
        {"Intrerupator Baie Oaspeti II"  , 12}, // 11
        {"Intrerupator Perete Dining III",  5}, // 12
        {"Intrerupator Perete Dining I"  ,  8}, // 13
        {"Intrerupator Dormitor"         ,  2}  // 14
    };
    RelaySettings relay[MAX_RELAYS] = {
        {"Lumina Balcon"           , false, true}, //  0
        {"Lumina Masa Dining"      , false, true}, //  1
        {"Lumina Dormitor"         , false, true}, //  2
        {"Lumina Hol Dormitor"     , false, true}, //  3
        {"Lumina Birou"            , false, true}, //  4
        {"Lumina Hue Lights"       , false, true}, //  5
        {"Spoturi Dining/Living"   , false, true}, //  6
        {"Lumina Dressing"         , false, true}, //  7
        {"Lumina Intrare"          , false, true}, //  8
        {"Ventilator Baie Dormitor", false, true}, //  9
        {"Lumina Baie Dormitor"    , false, true}, // 10
        {"Ventilator Baie Oaspeti" , false, true}, // 11
        {"Lumina Baie Oaspeti"     , false, true}, // 12
        {"NEUTILIZAT 13"           , false, true}, // 13
        {"NEUTILIZAT 14"           , false, true}, // 14
        {"Yala Usa Intrare"        , false, true}  // 15
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
    + JSON_ARRAY_SIZE(MAX_BUTTONS)     // ButtonSettingsArray
    + JSON_ARRAY_SIZE(MAX_RELAYS)      // RelaySettingsArray
    + MAX_RELAYS * JSON_OBJECT_SIZE(3)
    + MAX_BUTTONS * JSON_OBJECT_SIZE(2)
;

#endif
