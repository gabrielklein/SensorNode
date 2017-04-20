//
// This class is used for "notifications" on a RGB PIN
//
#ifndef RELAY_H
#define RELAY_H

#include "../Settings.h"
#include <Arduino.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"

class Relay : public IServer {
public:
Relay();
~Relay();
void setup();
String servName();
void servRegister(ESP8266WebServer *webServer);
void loop();
void servRelay();

private:
ESP8266WebServer *webServer;

};

#endif
