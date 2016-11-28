#ifndef SERVERUTIL_H
#define SERVERUTIL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class ServerUtil
{
public:

  static String getMime(String name);

private:
        ServerUtil() {
        }
};

#endif
