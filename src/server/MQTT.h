//
// This class is used for "notifications" on a RGB PIN
//
#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "../core/KeyStore.h"
#include <PubSubClient.h>

#include "../server/WebServerSN.h"
#include "../server/IServer.h"
#include "IMQTT.h"

#define MAX_IMQTT 20

class MQTT : public IServer {

public:
MQTT(FileServ *fileServ);
~MQTT();
void setup();
String servName();
void servRegister(ESP8266WebServer *webServer);
void loop();
void addServ(IMQTT *iMQTT);

private:
KeyStore keyStore;
ESP8266WebServer *webServer;
WiFiClient *espClient;
PubSubClient *client;
String macID;
IMQTT *imqtt[MAX_IMQTT];
int imqttCurrent = 0;
bool enable = false;
String topicChannel;

};

#endif
