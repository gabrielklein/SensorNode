//
// This class is used to detect switcg
//
#ifndef SWITCH_H
#define SWITCH_H

#include "../Settings.h"
#include <Arduino.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"

class Switch : public IServer {
public:
Switch();
~Switch();
void setup();
String servName();
void servRegister(ESP8266WebServer *webServer);
void loop();
void servSwitch();
int test = 0;

private:
ESP8266WebServer *webServer;

};

#endif
