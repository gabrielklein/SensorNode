#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <ESP8266WiFi.h>
#include "../Config.h"
#include "../notif/Notification.h"

class MyClient {


public:
        MyClient(Config* config, Notification *notification);
        ~MyClient();
        // Setup in the "setup" phase
        void setup();
        // Loop in the "loop phase"
        void loop();

private:
        Config* config;
        Notification *notification;

};

#endif
