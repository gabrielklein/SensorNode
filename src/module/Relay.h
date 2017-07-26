//
// This class is used for "notifications" on a RGB PIN
//
#ifndef RELAY_H
#define RELAY_H

#include "../Settings.h"
#include <Arduino.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"
#include "../server/IMQTT.h"

class Relay : public IServer, public IMQTT {
public:
Relay();
~Relay();
void setup();
String servName();
void servRegister(ESP8266WebServer *webServer);
void loop();
void servRelay();
void servSetMQTT(String mess);
String servGetMQTT();

private:
ESP8266WebServer *webServer;

};

#endif
