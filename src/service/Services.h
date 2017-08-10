//
// This class is used for "notifications" on a RGB PIN
//
#ifndef SERVICES_H
#define SERVICES_H

#include <Arduino.h>
#include "../core/Hub.h"
#include "../server/IServer.h"
#include "../core/KeyStore.h"

class Services : public IServer {

public:
Services(Hub* hub);
~Services();
void setup();
void loop();

String servName();
void servRegister(ESP8266WebServer *webServer);
void servGet();

private:
Hub* hub = NULL;
ESP8266WebServer *webServer;

};

#endif
