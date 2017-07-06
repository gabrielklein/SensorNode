
#ifndef SETTINGS_H
#define SETTINGS_H

// -------------------------

// Maximum number of items in config files
// Avoid using a crazy value
#define CONFIG_MAX_ITEM 32

// Webserver on port 80
#define WEB_SERVER_SN_ENABLE

// Client that open an access point or connect to the access point.
#define AP_SERVER_CLIENT_ENABLE

// Do we have a WS281x on port D4 (recommended)
#define WS281X_STRIP_ENABLE

// Switch
#define SWITCH_ENABLE
#define SWITCH_PIN_0 D5
#define SWITCH_PIN_1 D6
//#define SWITCH_PIN_2
//#define SWITCH_PIN_3
//#define SWITCH_PIN_4

// A dallas temperature module
#define TEMP_ENABLE
// Update every miliseconds temperature.
#define TEMP_UPDATE_EVERY_MS 15000
#define TEMP_DALLAS_PIN D1
#define TEMP_DALLAS_MAX_MODULE 8
#define TEMP_SECONDARY_PIN D3
#define TEMP_SECONDARY_TYPE DHT22
#define TEMP_SECONDARY_NAME "dht22"

// A relay, you can have up to 5 relays
// Comment pin assignement to avoid using them
#define RELAY_ENABLE
#define RELAY_PIN_0 D7
// If you enable it,
// don't forget to change the pin of the geiger counter (or disable it)
//#define RELAY_PIN_1 D8
//#define RELAY_PIN_2
//#define RELAY_PIN_3
//#define RELAY_PIN_4

// Geiger counter
#define GEIGER_ENABLE
#define GEIGER_PIN D8
// Conversion Factor, 0.00812 is for the J305ß tube
#define GEIGER_CONVERT_USV 0.00812


// Get time from internet and sync it locally.
#define TIME_ENABLED


// --------------------------------------------------------
// --------------------------------------------------------
// --------------------------------------------------------




#endif
