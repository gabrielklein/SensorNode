#ifndef MYWIFIAP_H
#define MYWIFIAP_H

#include <Arduino.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include "../Config.h"
#include "../notif/Notification.h"
#include "../util/ServerUtil.h"

class MyWifiAP {


public:
        MyWifiAP(Config* config, Notification *notification);
        ~MyWifiAP();
        // Setup in the "setup" phase
        void setup();
        // Loop in the "loop phase"
        void loop();

private:
        DNSServer dnsServer;
        ESP8266WebServer *webServer;
        // Ip of the module is stored here.
        char *ip;
        // Scan network and return result
        void getScan();
        // Get configuration
        void getConfig();
        // Update configuration
        void postConfig();
        // If we cannot process a page. "catch all". Respond with files
        void error();
        // Answer a page request
        void send(int responseCode, String content, String mime);
        // Reboot the ESP8266
        void doReboot();
        // Reset the configuration
        void doReset();

        Config *config;
        Notification *notif;
};

#endif
