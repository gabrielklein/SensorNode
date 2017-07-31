#include "Switch.h"

Switch::Switch() {

};

Switch::~Switch() {

};


void Switch::setup() {

        for (int i=0; i<SWITCH_MAX_COUNT; i++) {
                notifiedOn[i] = false;
        }

  #ifdef SWITCH_PIN_0
        pinMode(SWITCH_PIN_0, INPUT_PULLUP);
  #endif
  #ifdef SWITCH_PIN_1
        pinMode(SWITCH_PIN_1, INPUT_PULLUP);
  #endif
  #ifdef SWITCH_PIN_2
        pinMode(SWITCH_PIN_2, INPUT_PULLUP);
  #endif
  #ifdef SWITCH_PIN_3
        pinMode(SWITCH_PIN_3, INPUT_PULLUP);
  #endif
  #ifdef SWITCH_PIN_4
        pinMode(SWITCH_PIN_4, INPUT_PULLUP);
  #endif
};


String Switch::servName() {
        return "switch";
};

void Switch::loop() {

};

void Switch::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/switch/get", HTTP_GET, [&] () {
                this->servSwitch();
        });
};

void Switch::servSetMQTT(String mess) {
}

String Switch::servGetMQTT() {

  #ifdef SWITCH_PIN_0
        if (digitalRead(SWITCH_PIN_0) == HIGH) {
                if (notifiedOn[0]) {
                        notifiedOn[0] = false;
                        return "0 1";
                }
        }
        else {
                if (!notifiedOn[0]) {
                        notifiedOn[0] = true;
                        return "0 0";
                }
        }
  #endif

  #ifdef SWITCH_PIN_1
        if (digitalRead(SWITCH_PIN_1) == HIGH) {
                if (notifiedOn[1]) {
                        notifiedOn[1] = false;
                        return "1 1";
                }
        }
        else {
                if (!notifiedOn[1]) {
                        notifiedOn[1] = true;
                        return "1 0";
                }
        }
  #endif

  #ifdef SWITCH_PIN_2
        if (digitalRead(SWITCH_PIN_2) == HIGH) {
                if (notifiedOn[2]) {
                        notifiedOn[2] = false;
                        return "2 1";
                }
        }
        else {
                if (!notifiedOn[2]) {
                        notifiedOn[2] = true;
                        return "2 0";
                }
        }
  #endif

  #ifdef SWITCH_PIN_3
        if (digitalRead(SWITCH_PIN_3) == HIGH) {
                if (notifiedOn[3]) {
                        notifiedOn[3] = false;
                        return "3 1";
                }
        }
        else {
                if (!notifiedOn[3]) {
                        notifiedOn[3] = true;
                        return "3 0";
                }
        }
  #endif

  #ifdef SWITCH_PIN_4
        if (digitalRead(SWITCH_PIN_4) == HIGH) {
                if (notifiedOn[4]) {
                        notifiedOn[4] = false;
                        return "4 1";
                }
        }
        else {
                if (!notifiedOn[4]) {
                        notifiedOn[4] = true;
                        return "4 0";
                }
        }
  #endif

        return "";
}

void Switch::servSwitch() {


        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        JsonObject& l = root.createNestedObject("digital");

        #ifdef SWITCH_PIN_0
        if (digitalRead(SWITCH_PIN_0) == HIGH) {
                root["switch0"] = "1";
        }
        else {
                root["switch0"] = "0";
        }
        #endif

        #ifdef SWITCH_PIN_1
        if (digitalRead(SWITCH_PIN_1) == HIGH) {
                root["switch1"] = "1";
        }
        else {
                root["switch1"] = "0";
        }
        #endif

        #ifdef SWITCH_PIN_2
        if (digitalRead(SWITCH_PIN_2) == HIGH) {
                root["switch2"] = "1";
        }
        else {
                root["switch2"] = "0";
        }
        #endif

        #ifdef SWITCH_PIN_3
        if (digitalRead(SWITCH_PIN_3) == HIGH) {
                root["switch3"] = "1";
        }
        else {
                root["switch3"] = "0";
        }
        #endif

        #ifdef SWITCH_PIN_4
        if (digitalRead(SWITCH_PIN_4) == HIGH) {
                root["switch4"] = "1";
        }
        else {
                root["switch4"] = "0";
        }
        #endif

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

}
