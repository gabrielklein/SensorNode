//
// This class is used for "notifications" on a RGB PIN
//
#ifndef ITIME_H
#define ITIME_H

#include "../Settings.h"
#include "KeyStore.h"
#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <Time.h>
#include <Timezone.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"
#include "TZTime.h"

class ITime : public IServer {

public:
ITime(FileServ *fileServ);
~ITime();
void setup();
String servName();
void servRegister(ESP8266WebServer *webServer);
void servTime();
void servTimezone();
void loop();

private:
int tzCurrentIndex = -1;
ESP8266WebServer *webServer;
const char* timeHost = "time.nist.gov";
WiFiUDP ntpUDP;
NTPClient *timeClient;
TZTime tz;
String getFormattedTime(unsigned long rawTime);
KeyStore keyStore;
int getTimezoneIndex();
};

#endif
