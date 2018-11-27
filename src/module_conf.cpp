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

    // ButtonSettings
    JsonArray& button = root.createNestedArray("button");
    for(int i=0; i<MAX_BUTTONS; i++) {
        JsonObject& node        = button.createNestedObject();
        node["name"]            = settings.button[i].name;
        node["toggle_relay_id"] = settings.button[i].toggle_relay_id;
    }

    // RelaySettings
    JsonArray& relay = root.createNestedArray("relay");
    for(int i=0; i<MAX_RELAYS; i++) {
        JsonObject& node        = relay.createNestedObject();
        node["name"]            = settings.relay[i].name;
        node["default_state"]   = settings.relay[i].default_state;
        node["is_persistent"]   = settings.relay[i].is_persistent;
    }

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

    // RelaySettings - Array
    if (jsonSettings.containsKey("relay")) {
        JsonArray& jsonRelaySettingsArray = jsonSettings["relay"];

        // RelaySettings
        for(uint i=0; i < jsonRelaySettingsArray.size(); i++) {
            JsonObject& jsonRelaySettings = jsonRelaySettingsArray[i];

            // RelaySettings / name
            if (jsonRelaySettings.containsKey("name")) {
                strncpy(settings.relay[i].name, jsonRelaySettings["name"], sizeof(settings.relay[i].name));
            }

            // RelaySettings / default_state
            if (jsonRelaySettings.containsKey("default_state")) {
                settings.relay[i].default_state = jsonRelaySettings["default_state"];
            }

            // RelaySettings / is_persistent
            if (jsonRelaySettings.containsKey("is_persistent")) {
                settings.relay[i].is_persistent = jsonRelaySettings["is_persistent"];
            }
        }
    }

    // ButtonSettings - Array
    if (jsonSettings.containsKey("button")) {
        JsonArray& jsonButtonSettingsArray = jsonSettings["button"];

        // ButtonSettings
        for(uint i=0; i < jsonButtonSettingsArray.size(); i++) {
            JsonObject& jsonButtonSettings = jsonButtonSettingsArray[i];

            // ButtonSettings / name
            if (jsonButtonSettings.containsKey("name")) {
                strncpy(settings.button[i].name, jsonButtonSettings["name"], sizeof(settings.button[i].name));
            }

            // ButtonSettings / toggle_relay_id
            if (jsonButtonSettings.containsKey("toggle_relay_id")) {
                settings.button[i].toggle_relay_id = jsonButtonSettings["toggle_relay_id"];
            }
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