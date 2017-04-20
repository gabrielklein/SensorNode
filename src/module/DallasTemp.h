//
// This class is used for "notifications" on a RGB PIN
//
#ifndef DALLASTEMP_H
#define DALLASTEMP_H

#define DALLASTEMP_MAX_MODULE 10


#include "../Settings.h"
#include <Arduino.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"
#include <OneWire.h>
#include <DallasTemperature.h>

class DallasTemp : public IServer {
public:
DallasTemp();
~DallasTemp();
void setup();
String servName();
void servRegister(ESP8266WebServer *webServer);
void servTemp();
void loop();

private:
ESP8266WebServer *webServer;
OneWire *oneWire = NULL;
DallasTemperature *sensors = NULL;
int deviceCount = 0;
DeviceAddress addr[DALLASTEMP_MAX_MODULE];

};

#endif
