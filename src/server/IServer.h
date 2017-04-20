#ifndef ISERVER_H
#define ISERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>

class IServer {

public:
// Shortname of this service
virtual String servName();
// Add calls. Start with "shortname"
// Example: webServer->on("/ap/scan.do", HTTP_GET, [&] () {this->getScan(); });
virtual void servRegister(ESP8266WebServer *webServer);
};

#endif
