#include "module_mqtt.h"

MQTTClient mqttClient;

void mqtt_Connect           (void);
void mqtt_onMessageReceived (String &topic, String &payload);


void mqtt_setup(void) {
    // mqttClient.begin(settings.mqtt.broker, atoi(settings.mqtt.port), wifiClient);
    mqttClient.begin(settings.mqtt.broker, wifiClient);
    mqttClient.onMessage(mqtt_onMessageReceived);
    mqtt_Connect();
}

void mqtt_loop(void) {
    static unsigned long disconnectTime = 0;
    static bool disconnectFlag = false;

    if (!mqttClient.connected()) {
        if (disconnectFlag == false) {
            disconnectFlag = true;
            disconnectTime = millis();
            Log.warning("MQTT: Connection was dropped, will reconnect if not recovered in 1 sec");
        }
    } else {
        if (disconnectFlag == true) {
            Log.notice("MQTT: Connection recovered");
            disconnectFlag = false;
        }
    }

    if (!mqttClient.connected() && disconnectFlag && millis() - disconnectTime > 1000) {
        Log.warning("Connection dropped for more that 1 second, will try reconnect.");
        disconnectTime = millis();
        mqtt_Connect();
    }

    mqttClient.loop();
}

/**
 * @brief Connect to MQTT Server and perform some basic actions after
 * connecting:
 *  - subscribe to commands
 *  - send sensor and relay states
 * 
 */
void mqtt_Connect(void) {
    static uint32_t lastTryTime = 0;
    if (millis() - lastTryTime < 1000) {
        return;
    }
    lastTryTime = millis();

    Log.notice("MQTT: Trying to connect to %s:%s", settings.mqtt.broker, settings.mqtt.port);
    if(! mqttClient.connect(settings.mqtt.client_id)) {
        Log.warning("MQTT: Failed to connect, will try again later");
        return;
    }
    Log.notice("MQTT: Successfully connected to broker");

    mqttClient.subscribe((String) settings.mqtt.topic + "/cmd/#", 2);

    for (int i = 0; i < MAX_RELAYS; i++) {
        mqtt_statRelay(i, relayState[i]);
    }
    
    for (int i = 0; i < MAX_SENSORS; i++) {
        mqtt_statSensor(i, sensorState[i]);
    }
}

/**
 * @brief Handler for mqtt topics that we subscribed to
 * 
 * @param topic 
 * @param payload 
 */
void mqtt_onMessageReceived(String &topic, String &payload) {
    Log.notice("MQTT: Incomming message %s - %s", topic.c_str(), payload.c_str());

    if (topic.startsWith((String) settings.mqtt.topic + "/cmd/relay/")) {
        String relayIdString = topic;
        relayIdString.replace((String) settings.mqtt.topic + "/cmd/relay/", "");
        uint8_t relayId = relayIdString.toInt();

        if (payload == "on") {
            cmd_RelaySet(relayId, true);
        } else if (payload == "off") {
            cmd_RelaySet(relayId, false);
        } else {
            mqtt_statRelay(relayId, relayState[relayId]);
        }
    }
}

/**
 * @brief Publish the state of a relay
 * 
 * @param relayId 
 * @param state 
 */
void mqtt_statRelay(uint8_t relayId, bool state) {
    Log.notice("MQTT: Publishing /stat/relay/%d %s", relayId, state ? "on" : "off");
    mqttClient.publish((String) settings.mqtt.topic +"/stat/relay/" + relayId, state ? "on" : "off");
}

/**
 * @brief Publish the state of a sensor
 * 
 * @param sensorId 
 * @param state 
 */
void mqtt_statSensor(uint8_t sensorId, bool state) {
    Log.notice("MQTT: Publishing /stat/sensor/%d %s", sensorId, state ? "on" : "off");
    mqttClient.publish((String) settings.mqtt.topic + "/stat/sensor/" + sensorId, state ? "on" : "off");
}

void mqtt_statButton(uint8_t buttonId, uint8_t eventId) {
    String eventName = "unknown";

    switch(eventId) {
        case 1 : eventName = "click"    ; break;
        case 2 : eventName = "doble click" ; break;
        case 3 : eventName = "long press"; break;
        default: eventName = "unknown"  ; break;
    }

    Log.notice("MQTT: Publishing /stat/button/%d %s", buttonId, eventName.c_str());
    mqttClient.publish((String) settings.mqtt.topic + "/stat/button/" + buttonId, eventName);
}