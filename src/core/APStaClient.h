//
// This class is used to connect to the network.
//
#ifndef APSTACLIENT_H
#define APSTACLIENT_H

#include <Arduino.h>
#include "FS.h"
#include "FileServ.h"
#include "KeyStore.h"
#include "../module/Led.h"
#include "../server/IServer.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ArduinoJson.h>

class APStaClient : public IServer {

public:
APStaClient(FileServ *fileServ, Led *led);
~APStaClient();
bool setup();
void loop();
String servName();
void servRegister(ESP8266WebServer *webServer);
void servScan();
void servReset();
void servReboot();

private:
KeyStore keyStore;
bool connectClient();
bool connectAP();
DNSServer dnsServer;
bool isAPEnabled = false;
Led *led;
ESP8266WebServer *webServer;
FileServ *fileServ;
};

#endif
