//
// This class is used for "notifications" on a RGB PIN
//
#ifndef GEIGER_H
#define GEIGER_H

#define MAX_STORE_DATA_GEIGER 600

#include "../Settings.h"
#include <Arduino.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"
#include "../core/KeyStore.h"

class Geiger : public IServer {
public:
Geiger(FileServ *fileServ);
~Geiger();
KeyStore keyStore;
void setup();
String servName();
void servRegister(ESP8266WebServer *webServer);
void loop();
void tubeImpulse();
int count;
void servGet();

const float conversionFactor = GEIGER_CONVERT_USV;

// We store data here in a circular buffer
// So we store MAX_STORE_DATA seconds of data
// Bug if we receive no data during MAX_STORE_DATA_GEIGER. Buffer is not erased
int coutGeigerPerSecond[MAX_STORE_DATA_GEIGER];

private:
ESP8266WebServer *webServer;
unsigned long startMillis;
int currentCountGeigerPointer = 0;
double hitPerSecsTouSv(double hitPerSeconds);
};

#endif
