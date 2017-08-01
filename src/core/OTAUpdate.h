#ifndef OTAUPDATE_H
#define OTAUPDATE_H

#include <Arduino.h>
#include <string.h>
#include "FileServ.h"
#include <ESP8266WebServer.h>

#include <WiFiUdp.h>
#include <ArduinoOTA.h>

class OTAUpdate {

public:
// Construct using the notification object
OTAUpdate();
~OTAUpdate();

void loop();
void setup();


};
#endif
