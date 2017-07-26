#ifndef KEYSTORE_H
#define KEYSTORE_H

#include <Arduino.h>
#include <string.h>
#include "FileServ.h"
#include <ESP8266WebServer.h>
#include "../server/ServerUtil.h"
#include "../Settings.h"

class KeyStore {

public:
// Construct using the notification object
KeyStore();
~KeyStore();
// Save configuration
bool saveKeyStore();
// Setup fileserv
bool setup(String name, FileServ* fileServ);
// Get a value in the configuration
String getValue(String key);
// Get a value in the configuration
int getValueInt(String key);
// Get a value in the configuration
float getValueFloat(String key);
// Get a value in the configuration
bool getValueBool(String key);
// Set a value in the configuration
bool setValue(String key, String value);
// Set a value in the configuration
void servConfig(ESP8266WebServer *webServer);

private:
String key[CONFIG_MAX_ITEM];
String value[CONFIG_MAX_ITEM];
String path;
FileServ *fileServ;
String filePath;
};

#endif
