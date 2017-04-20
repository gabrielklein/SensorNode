//
// First configure "Setting.h", then start this file.
//
#include <ESP8266WiFi.h>
#include "Settings.h"
#include "core/Hub.h"

Hub* hub = NULL;

void setup()
{
        hub = new Hub();
        hub->setup();
}


int kk = 0;

void loop()
{
        hub->loop();
}
