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
#include "../module/Led.h"
#include "../module/Temp.h"
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
Led *led = NULL;
Temp *temp = NULL;
Relay *relay = NULL;
ITime *iTime = NULL;
bool isClientMode = false;
};

#endif
