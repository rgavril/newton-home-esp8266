#include "module_ota.h"

void ota_setup(void) {
    Log.notice("OTA: Configuring OTA updates");

    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Log.notice("OTA: Update started for %s", type.c_str());
    });

    ArduinoOTA.onEnd([]() {
        Log.notice("OTA: Update finished");
    });

    // ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //     Log.notice("OTA: Update progress %u %u", progress, total);
    // });

    ArduinoOTA.onError([](ota_error_t error) {
        
        if (error == OTA_AUTH_ERROR) {
            Log.error("OTA: Error %u - Auth Failed", error);
        } else if (error == OTA_BEGIN_ERROR) {
            Log.error("OTA: Error %u - Begin Failed", error);
        } else if (error == OTA_CONNECT_ERROR) {
            Log.error("OTA: Error %u - Connect Failed", error);
        } else if (error == OTA_RECEIVE_ERROR) {
            Log.error("OTA: Error %u - Receive Failed", error);
        } else if (error == OTA_END_ERROR) {
            Log.error("OTA: Error %u - End Failed", error);
        }
    });

    ArduinoOTA.begin();
}

void ota_loop(void) {
    ArduinoOTA.handle();
}