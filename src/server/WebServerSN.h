#ifndef WEBSERVERSN_H
#define WEBSERVERSN_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "../core/FileServ.h"
#include "../server/IServer.h"

class WebServerSN : public IServer  {

public:
WebServerSN(FileServ *fileServ);
~WebServerSN();
void loop();
void setup();
void addServ(IServer *iServer);
String servName();
void servRegister(ESP8266WebServer *webServer);

private:
FileServ *fileServ;
ESP8266WebServer *webServer;
void servFiles();
void send(int responseCode, String content, String mime);

};

#endif
