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
#include "../server/IMQTT.h"

class Temp : public IServer, public IMQTT {
public:
Temp();
~Temp();
void setup();
String servName();
void servRegister(ESP8266WebServer *webServer);
void servTemp();
void loop();
void servSetMQTT(String mess);
String servGetMQTT();

private:
ESP8266WebServer *webServer;
#ifdef TEMP_DALLAS_ENABLE
OneWire *oneWire = NULL;
DallasTemperature *sensors = NULL;
int deviceCount = 0;
DeviceAddress addr[TEMP_DALLAS_MAX_MODULE];
unsigned long lastMQTTMessDallas[TEMP_DALLAS_MAX_MODULE];
String addr2MType(DeviceAddress* addr);
String addr2Hex(DeviceAddress* addr);
#endif
#ifdef TEMP_SECONDARY_ENABLE
DHT *dht;
unsigned long lastMQTTMessDHT = 0;
#endif
char hexC[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
long timeLastUpdate = 0;
float dhtTemp = 0;
float dhtHumidity = 0;
};

#endif
