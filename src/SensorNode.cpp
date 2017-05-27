//
// First configure "Setting.h", then start this file.
//
#include <ESP8266WiFi.h>
#include "Settings.h"
#include "core/Hub.h"
#include "service/Services.h"

Hub* hub = NULL;
Services* services = NULL;

void setup()
{
        hub = new Hub();
        hub->setup();
        services = new Services(hub);
        services->setup();
}


int kk = 0;

void loop()
{
        hub->loop();
        services->loop();
}
