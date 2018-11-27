#include "module_web.h"

ESP8266WebServer server(80);

void web_handleNotFound    (void);
bool web_handleFileRead    (String path);
void web_handleRelaysGet   (void);
void web_handleSettingsGet (void);
void web_handleSettingsSet (void);

void web_setup(void) {
    server.on("/relays/get"  , web_handleRelaysGet);
    server.on("/settings/get", web_handleSettingsGet);
    server.on("/settings/set", web_handleSettingsSet);

    
    server.onNotFound([]() {
        if (! web_handleFileRead("/web" + server.uri()) ) {
            web_handleNotFound();
        }
    });

    server.begin();

    Log.notice("WEB: Server started on ip %s", WiFi.localIP().toString().c_str());
}

void web_loop(void) {
    server.handleClient();
}

/**
 * @brief Page not found
 * 
 */
void web_handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, "text/plain", message);
}

/**
 * @brief Generate a json containing the relay states
 * 
 */
void web_handleRelaysGet() {
    const size_t bufferSize = MAX_RELAYS*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(MAX_RELAYS);
    DynamicJsonBuffer jsonBuffer(bufferSize);

    JsonObject& root = jsonBuffer.createObject();

    for (int relayId = 0; relayId < MAX_RELAYS; relayId++) {
        JsonObject& relayJson = root.createNestedObject((String)"relay"+relayId);
        relayJson["id"]    = relayId;
        relayJson["state"] = relayState[relayId];
    }

    String json;
    root.printTo(json);
    server.send(200, "text/json", json);
}


/**
 * @brief 
 * 
 */
void web_handleSettingsGet() {
    if (! web_handleFileRead("/settings.json") ) {
        web_handleNotFound();
    }
}

void web_handleSettingsSet() {
    DynamicJsonBuffer jsonBuffer(SETTINGS_JSON_BUFFER_SIZE + 300);
    JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
    
    JsonObject& jsonSettings = root["settings"];
    
    conf_SettingsFromJsonObject(jsonSettings);
    conf_SaveSettings();

    server.send(200, "text/plain", "OK");

    ESP.restart();
}
/**
 * @brief 
 * 
 * @param path 
 * @return true 
 * @return false 
 */
bool web_handleFileRead(String path) {
    if (path.endsWith("/")) {
        path += "index.html";
    }

    String contentType = "text/plain";
    if      (path.endsWith(".html")) { contentType = "text/html";              }
    else if (path.endsWith(".htm" )) { contentType = "text/html";              }
    else if (path.endsWith(".css" )) { contentType = "text/css";               }
    else if (path.endsWith(".js"  )) { contentType = "application/javascript"; }
    else if (path.endsWith(".json")) { contentType = "application/json";       }
	else if (path.endsWith(".png" )) { contentType = "image/png";              }
	else if (path.endsWith(".gif" )) { contentType = "image/gif";              }
	else if (path.endsWith(".jpg" )) { contentType = "image/jpeg";             }
	else if (path.endsWith(".ico" )) { contentType = "image/x-icon";           }
	else if (path.endsWith(".xml" )) { contentType = "text/xml";               }
	else if (path.endsWith(".pdf" )) { contentType = "application/x-pdf";      }
	else if (path.endsWith(".zip" )) { contentType = "application/x-zip";      }
	else if (path.endsWith(".gz"  )) { contentType = "application/x-gzip";     }


    File file = SPIFFS.open(path, "r");
    if (!file) {
        Log.warning("WEB: Requested file '%s' not found on spiffs.", path.c_str());
        return false;
    }

    server.streamFile(file, contentType);
    file.close();

    return true;
}