#include "ServerUtil.h"

/**
 * Get mime typed based on the extension of the file.
 */
String ServerUtil::getMime(String name) {

        if (name.equals("/")) {
                return "text/html";
        }
        else if (name.endsWith(".png")) {
                return "image/png";
        }
        else if (name.endsWith(".jpg")) {
                return "image/jpeg";
        }
        else if (name.endsWith(".js")) {
                return "application/javascript";
        }
        else if (name.endsWith(".css")) {
                return "text/css";
        }
        else if (name.endsWith(".html")) {
                return "text/html";
        }
        else if (name.endsWith(".txt")) {
                return "text/plain";
        }
        else if (name.endsWith(".json")) {
                return "application/json";
        }
        else if (name.endsWith(".xml")) {
                return "application/xml";
        }

        return "text/html";
};

/**
 * Send a success message
 */
void ServerUtil::sendSuccess(ESP8266WebServer *webServer) {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& x = jsonBuffer.createObject();
        x["status"] = "ok";
        x["time"] = millis();

        String s;
        x.printTo(s);
        webServer->send(200, "application/json", s);
};

/**
 * Send a fail message
 */
void ServerUtil::sendFail(ESP8266WebServer *webServer, int errorId, String errorDesc) {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& x = jsonBuffer.createObject();
        x["status"] = "error";
        x["id"] = errorId;
        x["desc"] = errorDesc;
        x["time"] = millis();

        String s;
        x.printTo(s);
        webServer->send(200, "application/json", s);
};
