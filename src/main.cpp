#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "globals.h"
#include "module_conf.h"
#include "module_cmd.h"
#include "module_web.h"
#include "module_mqtt.h"
#include "module_ota.h"

Settings   settings;
WiFiClient wifiClient;
bool       relayState[MAX_RELAYS];
bool       sensorState[MAX_SENSORS];

void setup() {
    // Serial
    Serial.begin(115200);

    // Filesystem
    SPIFFS.begin();
    
    // Logging
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    Log.setSuffix([](Print* _logOutput) { _logOutput->print('\n'); });

    // Welcome Messages
    Log.notice("Version : 2");
    Log.notice("Booting %s, ", settings.network.hostname);
    Log.notice("Flash size: %d MB.", ESP.getFlashChipRealSize()/(1024*1024));
    Log.notice("CPU speed: %d MHz.", ESP.getCpuFreqMHz());
    Log.notice("Restart Reason: %s.", ESP.getResetReason().c_str());
    ESP.getFlashChipRealSize();

    // Load settings before doing anything else
    conf_LoadSettings();
    conf_SaveSettings();

    // Wifi
    WiFi.begin(settings.network.ssid, settings.network.password);
    Log.notice("Connecting to Wi-Fi network '%s'", settings.network.ssid);
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    Log.notice("Connected with IP Address %s", WiFi.localIP().toString().c_str());

    // MDNS
    if (MDNS.begin(settings.network.hostname)) {
        Log.notice("MDNS reponder started");
	}

    ota_setup();
    cmd_setup();
    mqtt_setup();
    web_setup();

    // Trigger a update of the relay and sensor states
    cmd_RelaysGet();
    cmd_SensorsGet();
}

void loop() {
    ota_loop();
    cmd_loop();
    mqtt_loop();
    web_loop();
}