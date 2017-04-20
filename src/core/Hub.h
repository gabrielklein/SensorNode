//
// This class is used for "notifications" on a RGB PIN
//
#ifndef HUB_H
#define HUB_H

#include <Arduino.h>

#include "FileServ.h"
#include "APStaClient.h"
#include "ITime.h"
#include "../server/WebServerSN.h"
#include "../module/WS281xStrip.h"
#include "../module/DallasTemp.h"
#include "../module/Relay.h"


class Hub {

public:
Hub();
~Hub();
void setup();
void loop();

private:
WebServerSN *webServerSN = NULL;
FileServ fileServ;
APStaClient *apStaClient = NULL;
WS281xStrip *ws281xStrip = NULL;
DallasTemp *dallasTemp = NULL;
Relay *relay = NULL;
ITime *iTime = NULL;
};

#endif
