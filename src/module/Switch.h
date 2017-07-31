//
// This class is used to detect switcg
//
#ifndef SWITCH_H
#define SWITCH_H

#define SWITCH_MAX_COUNT 5

#include "../Settings.h"
#include <Arduino.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"
#include "../server/IMQTT.h"

class Switch : public IServer, public IMQTT {
public:
Switch();
~Switch();
void setup();
String servName();
void servRegister(ESP8266WebServer *webServer);
void loop();
void servSwitch();
int test = 0;
void servSetMQTT(String mess);
String servGetMQTT();

private:
ESP8266WebServer *webServer;
boolean notifiedOn[SWITCH_MAX_COUNT];

};

#endif
