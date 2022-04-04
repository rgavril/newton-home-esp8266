#include "module_cmd.h"

SerialCommand serialCommand;

void cmd_onRelayNotify   (void);
void cmd_onSensorNotify  (void);
void cmd_onRelaysNotify  (void);
void cmd_onSensorsNotify (void);
void cmd_onButtonNotify  (void);

/**
 * @brief This function is called when Arudino Mega notified us trough
 * serial bus that one of it's relays changed state.
 * 
 * @RELAY_NOTIFY <relayId> <state>
 */
void cmd_onButtonNotify(void) {
    char *arg1 = serialCommand.next(); if (arg1 == NULL) return;
    char *arg2 = serialCommand.next(); if (arg2 == NULL) return;

    uint8_t buttonId = atoi(arg1); 
    uint8_t eventId  = atoi(arg2);

    Log.notice("CMD: Button %d triggered event %d ( @BUTTON_NOTIFY %s %s )", buttonId, eventId, arg1, arg2);

    mqtt_statButton(buttonId, eventId);
}

/**
 * @brief This function is called when Arudino Mega notified us trough
 * serial bus that one of it's relays changed state.
 * 
 * @RELAY_NOTIFY <relayId> <state>
 */
void cmd_onRelayNotify(void) {
    char *arg1 = serialCommand.next(); if (arg1 == NULL) return;
    char *arg2 = serialCommand.next(); if (arg2 == NULL) return;

    uint8_t relayId = atoi(arg1); 
    if (relayId >= MAX_RELAYS) {
        return;
    }

    bool state = atoi(arg2);

    Log.notice("CMD: Setting relay %d to %d ( @RELAY_NOTIFY %s %s )", relayId, state, arg1, arg2);

    relayState[relayId] = state;
    mqtt_statRelay(relayId, state);
}

/**
 * @brief This function is called when Arudino Mega notified us trough
 * serial bus that one of it's sensors changed state.
 * 
 * @SENSOR_NOTIFY <sensorId> <state>
 */
void cmd_onSensorNotify(void) {
    Serial.println("HERE");
    char *arg1 = serialCommand.next(); if (arg1 == NULL) return;
    char *arg2 = serialCommand.next(); if (arg2 == NULL) return;

    uint8_t sensorId = atoi(arg1); 
    if (sensorId >= MAX_SENSORS) { return; }

    bool state = atoi(arg2);

    Log.notice("CMD: Setting sensor %d to %d ( @SENSOR_NOTIFY %s %s )", sensorId, state, arg1, arg2);

    sensorState[sensorId] = state;
    mqtt_statSensor(sensorId, state);
}

/**
 * @brief This function is called when Arudino Mega notified us trough
 * serial bus of the states of it's relays. Usually this comes as a reply
 * to the @RELAYS_GET command.
 * 
 * The relayStates is encoded as a 32bit usined integer with each bit
 * representing the boolean state of a relay.
 * 
 * @RELAYS_NOTIFY <relayStates>
 */
void cmd_onRelaysNotify(void) {
    char *arg1 = serialCommand.next(); if (arg1 == NULL) return;
    
    uint32_t states = atoi(arg1);

    Log.notice("CMD: Received relay states ( @RELAYS_NOTIFY %s )", arg1);

    for(int relayId = 0; relayId < MAX_RELAYS; relayId++) {
        bool state = states & 1 << relayId;

        if (state != relayState[relayId]) {
            Log.notice("CMD: Setting relay %d to %d ( @RELAYS_NOTIFY %s )", relayId, state, arg1);

            relayState[relayId] = state;
            mqtt_statRelay(relayId, state);
        }
    }
}

/**
 * @brief This function is called when Arudino Mega notified us trough
 * serial bus of the states of it's sensors. Usually this comes as a reply
 * to the @SENSORS_GET command.
 * 
 * The sensorsStates is encoded as a 32bit usined integer with each bit
 * representing the boolean state of a sensor.
 * 
 * @SENSORS_NOTIFY <sensorsStates>
 */
void cmd_onSensorsNotify(void) {
    char *arg1 = serialCommand.next(); if (arg1 == NULL) return;

    uint32_t states = atoi(arg1);

    Log.notice("CMD: Received sensor states ( @SENSORS_NOTIFY %s )", arg1);

    for(int sensorId = 0; sensorId < MAX_SENSORS; sensorId++) {
        bool state = states & 1 << sensorId;

        if (state != sensorState[sensorId]) {
            Log.notice("CMD: Setting sensor %d to %d ( @SENSORS_NOTIFY %s )", sensorId, state, arg1);

            sensorState[sensorId] = state;
            mqtt_statSensor(sensorId, state);
        }
    }
}

/**
 * @brief Change the state of one of Arduino's controlled relays by sending
 * this comand trough the serial bus. On success this should be fallowed
 * by a response with @RELAY_NOTIFY from arduino.
 *  
 * @param relayId  id of the relay we want to change
 * @param state    true for on, false for off
 * 
 * @RELAY_SET <relayId> <state>
 */
void cmd_RelaySet(int relayId, bool state) {
    Serial.println((String) "\n@RELAY_SET " + relayId + " " + state);
}

void cmd_RelaysGet(void) {
    Serial.println((String) "\n@RELAYS_GET");
}

void cmd_SensorsGet(void) {
    Serial.println((String) "\n@SENSORS_GET");
}

void cmd_setup() {
    serialCommand.addCommand("@RELAY_NOTIFY"  , cmd_onRelayNotify);
    serialCommand.addCommand("@SENSOR_NOTIFY" , cmd_onSensorNotify);
    serialCommand.addCommand("@RELAYS_NOTIFY" , cmd_onRelaysNotify);
    serialCommand.addCommand("@SENSORS_NOTIFY", cmd_onSensorsNotify);
    serialCommand.addCommand("@BUTTON_NOTIFY" , cmd_onButtonNotify);
}

void cmd_loop() {
    serialCommand.readSerial();
}