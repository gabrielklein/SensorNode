//
// This class is used for "notifications" on a RGB PIN
//
#ifndef WS281XSTRIP_H
#define WS281XSTRIP_H


#include "../Settings.h"
#include <Arduino.h>
#include <NeoPixelBus.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"

class WS281xStrip : public IServer {
public:
WS281xStrip();
~WS281xStrip();
void rgb(int r, int g, int b);
void rgb(int id, int r, int g, int b);
void setup();
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> *neo;
String servName();
void servRegister(ESP8266WebServer *webServer);
void servSet();
int countLed = 60;
void loop();

private:
ESP8266WebServer *webServer;

};

#endif
