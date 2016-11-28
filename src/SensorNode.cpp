//
// First configure "Setting.h", then start this file.
//
#include <ESP8266WiFi.h>
#include "Settings.h"
#include "Config.h"
#include "ap/MyWifiAP.h"
#include "client/MyClient.h"
#include "notif/Notification.h"
#include "notif/RGBNotification.h"
#include "notif/NoNotification.h"

// Store configuration
Config* config = NULL;
// Store "notifications"
Notification* notification = NULL;
// Used for access point mode
MyWifiAP* myWifiAP = NULL;
// Used for client mode
MyClient* myClient = NULL;

void setup()
{

        // We want to be able to read files
        SPIFFS.begin();

        // Use serial
        Serial.begin ( 115200 );

        // Initialize AP mode
      #ifdef AP_MODE_SWITCH
        pinMode(AP_MODE_RESET_WLAN, INPUT);
        int r = digitalRead(AP_MODE_RESET_WLAN);
      #else
        int r = 0;
      #endif

        // Create a notification object
      #ifdef NOTIFICATION_LED_RGB
        notification = new RGBNotification(D2, D3, D4);
      #else
        notification = new NoNotification();
      #endif
        notification->setup();

        // Initialize config
        config = new Config(notification);
        //config->setup();

        // When the "reset button" is enabled - or no config, activate the "AP mode"
        if (r == 1 || config->getValue("wireless_ssid") == NULL || config->getValue("wireless_ssid").equals("")) {
                myWifiAP = new MyWifiAP(config, notification);
                myWifiAP->setup();
        }
        // Otherwise start as "client mode"
        else {
                myClient = new MyClient(config, notification);
                myClient->setup();
        }

}

void loop()
{

        //if (config != NULL) {
        //        config->loop();
        //}

        if (notification != NULL) {
                notification->loop();
        }

        // We are in "AP mode"
        if (myWifiAP != NULL) {
                myWifiAP->loop();
        }

        // We are in "AP mode"
        if (myClient != NULL) {
                myClient->loop();
        }

}
