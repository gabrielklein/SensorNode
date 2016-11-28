#include <ESP8266WiFi.h>

#ifndef SETTINGS_H
#define SETTINGS_H

// -------------------------
// Define the AP Mode "switch"

// If we have a power switch on pin D1 (pull up)
#define AP_MODE_SWITCH
static const uint8_t AP_MODE_RESET_WLAN = D1;

// -------------------------
// Define the AP Mode "switch"

// If we have a RGB notification led
#define NOTIFICATION_LED_RGB
static const uint8_t NOTIFICATION_LED_R = D2;
static const uint8_t NOTIFICATION_LED_G = D3;
static const uint8_t NOTIFICATION_LED_B = D4;




#endif
