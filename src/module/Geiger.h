//
// This class is used for "notifications" on a RGB PIN
//
#ifndef GEIGER_H
#define GEIGER_H

#define MAX_STORE_DATA_GEIGER 300

#include "../Settings.h"
#include <Arduino.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"
#include "../core/KeyStore.h"
#include "../server/IMQTT.h"

class Geiger : public IServer, public IMQTT {
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
void servSetMQTT(String mess);
String servGetMQTT();


const float conversionFactor = GEIGER_CONVERT_USV;

// We store data here in a circular buffer
// So we store MAX_STORE_DATA seconds of data
// Bug if we receive no data during MAX_STORE_DATA_GEIGER. Buffer is not erased
int coutGeigerPerSecond[MAX_STORE_DATA_GEIGER];

private:
ESP8266WebServer *webServer;
unsigned long startMillis;
int currentCountGeigerPointer = 0;
float hitPerSecsTouSv(float hitPerSeconds);
unsigned long lastMQTTMess = 0;
int updateCounters();

int count5 = 0;
int count15 = 0;
int count60 = 0;
int count300 = 0;

float count5f = 0;
float count15f = 0;
float count60f = 0;
float count300f = 0;

};

#endif
