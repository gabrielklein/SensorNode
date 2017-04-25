//
// This class is used for "notifications" on a RGB PIN
//
#ifndef TEMP_H
#define TEMP_H

#define TEMP_MAX_MODULE 10


#include "../Settings.h"
#include <Arduino.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#include <DHT.h>
#include <DHT_U.h>

class Temp : public IServer {
public:
Temp();
~Temp();
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
DeviceAddress addr[TEMP_DALLAS_MAX_MODULE];
DHT *dht;
long timeLastUpdate = 0;
float dhtTemp = 0;
float dhtHumidity = 0;
};

#endif
