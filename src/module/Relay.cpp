#include "Relay.h"

Relay::Relay() {

};

Relay::~Relay() {

};


void Relay::setup() {
  #ifdef RELAY_PIN_0
        pinMode(RELAY_PIN_0, OUTPUT);
        digitalWrite(RELAY_PIN_0,LOW);
  #endif
  #ifdef RELAY_PIN_1
        pinMode(RELAY_PIN_1, OUTPUT);
        digitalWrite(RELAY_PIN_1,LOW);
  #endif
  #ifdef RELAY_PIN_2
        pinMode(RELAY_PIN_2, OUTPUT);
        digitalWrite(RELAY_PIN_2,LOW);
  #endif
  #ifdef RELAY_PIN_3
        pinMode(RELAY_PIN_3, OUTPUT);
        digitalWrite(RELAY_PIN_3,LOW);
  #endif
  #ifdef RELAY_PIN_4
        pinMode(RELAY_PIN_4, OUTPUT);
        digitalWrite(RELAY_PIN_4,LOW);
  #endif



};


String Relay::servName() {
        return "relay";
};

void Relay::loop() {

};

void Relay::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/relay/set", HTTP_GET, [&] () {this->servRelay(); });
};

void Relay::servRelay() {

        int relay = -9999;
        bool status = false;

        int currentArgCount = this->webServer->args();
        for (int i = 0; i < currentArgCount; ++i) {
                String key = this->webServer->argName(i);
                String value = this->webServer->arg(i);
                if (value == NULL) {
                        value = "";
                }

                if (key.equals("r")) {
                        relay = value.toInt();
                }
                if (key.equals("v")) {
                        status = value.equals("1") || value.equals("on");
                }
        }

        int pin = -9999;

        #ifdef RELAY_PIN_4
        if (relay == 4) {
                pin = RELAY_PIN_4;
        }
        #endif

        #ifdef RELAY_PIN_3
        if (relay == 3) {
                pin = RELAY_PIN_3;
        }
        #endif

        #ifdef RELAY_PIN_2
        if (relay == 2) {
                pin = RELAY_PIN_2;
        }
        #endif

        #ifdef RELAY_PIN_1
        if (relay == 1) {
                pin = RELAY_PIN_1;
        }
        #endif

        #ifdef RELAY_PIN_0
        if (relay == 0) {
                pin = RELAY_PIN_0;
        }
        #endif

        if (pin == -9999) {
                ServerUtil::sendFail(this->webServer, 2, "Relay id is not define or invalid.");
                return;
        }

        digitalWrite(pin,status ? HIGH : LOW);
        Serial.printf("Set relay %d on pin %d to status %d\n", relay, pin, status);

        ServerUtil::sendSuccess(this->webServer);
}
