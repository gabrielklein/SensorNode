//
// This class is used for "notifications" on a RGB PIN
//
#ifndef LED_H
#define LED_H


#include "../Settings.h"
#include <Arduino.h>
#include <NeoPixelBus.h>
#include "../server/IServer.h"
#include "../server/ServerUtil.h"
#include "../core/KeyStore.h"

class Led : public IServer {
public:
Led(FileServ *fileServ);
~Led();
KeyStore keyStore;
void rgb(int r, int g, int b, bool isLoopSetupTask);
void rgb(int id, int r, int g, int b);
void setup();
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart800KbpsMethod> *neo;
String servName();
void servRegister(ESP8266WebServer *webServer);
void servSet();
void servGet();
int countLed;
void ledCountUpdated();
void loop();
int counter1 = 0;
int counter2 = 0;
int counter3 = 0;
void buildinLed(bool ison);


private:
ESP8266WebServer *webServer;
bool doCallLedCountUpdated = false;
bool doCallNeoShow = false;
const short int BUILTIN_LED1 = 2;

};

#endif
