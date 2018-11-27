#include "module_conf.h"

void conf_SaveSettings(void) 
{
    Log.notice("CONF: Saving settings in 'settings.json'");

    DynamicJsonBuffer jsonBuffer(SETTINGS_JSON_BUFFER_SIZE);

    JsonObject& root = jsonBuffer.createObject();

    // NetworkSettings
    JsonObject& network = root.createNestedObject("network");
    network["ssid"]     = settings.network.ssid;
    network["password"] = settings.network.password;
    network["hostname"] = settings.network.hostname;

    // MQTTSettings
    JsonObject& mqtt  = root.createNestedObject("mqtt");
    mqtt["broker"]    = settings.mqtt.broker;
    mqtt["port"]      = settings.mqtt.port;
    mqtt["client_id"] = settings.mqtt.client_id;
    mqtt["topic"]     = settings.mqtt.topic;
    mqtt["user"]      = settings.mqtt.user;
    mqtt["password"]  = settings.mqtt.password;

    File file = SPIFFS.open("/settings.json", "w");
    if (!file) {
        Log.error("CONF: Cannot open 'settings.json' for writing");
        return;
    }

    root.prettyPrintTo(file);
    file.close();
}

void conf_LoadSettings(void) 
{
    Log.notice("CONF: Loading settings from 'settings.json'");

    File file = SPIFFS.open("/settings.json", "r");
    if (!file) {
        Log.warning("CONF: Cannot open 'settings.json' for reading");
        return;
    }

    DynamicJsonBuffer jsonBuffer(SETTINGS_JSON_BUFFER_SIZE + 500);

    JsonObject& jsonSettings = jsonBuffer.parseObject(file);

    conf_SettingsFromJsonObject(jsonSettings);
}

void conf_SettingsFromJsonObject(JsonObject &jsonSettings) {
    // NetworkSettings
    if (jsonSettings.containsKey("network")) {
        JsonObject& jsonNetworkSettings = jsonSettings["network"];
        
        // NetworkSettings / ssid
        if (jsonNetworkSettings.containsKey("ssid")) {
            strncpy(settings.network.ssid, jsonNetworkSettings["ssid"], sizeof(settings.network.ssid));
        }

        // NetworkSettings / password
        if (jsonNetworkSettings.containsKey("password")) {
            strncpy(settings.network.password, jsonNetworkSettings["password"], sizeof(settings.network.password));
        }
        
        // NetworkSettings /hostname
        if (jsonNetworkSettings.containsKey("hostname")) {
            strncpy(settings.network.hostname, jsonNetworkSettings["hostname"], sizeof(settings.network.hostname));
        }
    }

    // MQTTSettings
    if (jsonSettings.containsKey("mqtt")) {
        JsonObject& jsonMQTTSettings = jsonSettings["mqtt"];

        // MQTTSettings / broker
        if (jsonMQTTSettings.containsKey("broker")) {
            strncpy(settings.mqtt.broker, jsonMQTTSettings["broker"], sizeof(settings.mqtt.broker));
        }

        // MQTTSettings / port
        if (jsonMQTTSettings.containsKey("port")) {
            strncpy(settings.mqtt.port, jsonMQTTSettings["port"], sizeof(settings.mqtt.port));
        }

        // MQTTSettings / client_id
        if (jsonMQTTSettings.containsKey("client_id")) {
            strncpy(settings.mqtt.client_id, jsonMQTTSettings["client_id"], sizeof(settings.mqtt.client_id));
        }

        // MQTTSettings / topic
        if (jsonMQTTSettings.containsKey("topic")) {
            strncpy(settings.mqtt.topic, jsonMQTTSettings["topic"], sizeof(settings.mqtt.topic));
        }

        // MQTTSettings / user
        if (jsonMQTTSettings.containsKey("user")) {
            strncpy(settings.mqtt.user, jsonMQTTSettings["user"], sizeof(settings.mqtt.user));
        }

        // MQTTSettings / password
        if (jsonMQTTSettings.containsKey("password")) {
            strncpy(settings.mqtt.password, jsonMQTTSettings["password"], sizeof(settings.mqtt.password));
        }
    }
}

void conf_FactoryReset() {
    Settings factorySettings;

    sprintf(factorySettings.network.hostname, "arduino-%08X", ESP.getChipId());

    sprintf(factorySettings.mqtt.port       , "1833");
    sprintf(factorySettings.mqtt.broker     , "10.0.1.7");
    sprintf(factorySettings.mqtt.client_id  , "arduino-%08X", ESP.getChipId());
    sprintf(factorySettings.mqtt.topic      , "arduino-%08X", ESP.getChipId());
    
    settings = factorySettings;
    conf_SaveSettings();
}