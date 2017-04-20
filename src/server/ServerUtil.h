#ifndef SERVERUTIL_H
#define SERVERUTIL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class ServerUtil
{
public:

static String getMime(String name);
static void sendSuccess(ESP8266WebServer *webServer);
static void sendFail(ESP8266WebServer *webServer, int errorId, String errorDesc);

private:
ServerUtil() {
}
};

#endif
