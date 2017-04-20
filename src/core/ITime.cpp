#include "ITime.h"

ITime::ITime() {
        this->timeClient = new NTPClient(this->ntpUDP, this->timeHost, 0, 60*60*1000 /*Once an hour*/);
};

ITime::~ITime() {
        delete(this->timeClient);
};

void ITime::setup() {
        timeClient->begin();
};

String ITime::servName() {
        return "time";
};

void ITime::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/time/now", HTTP_GET, [&] () {this->servTime(); });
};


void ITime::servTime() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();

        root["date"] =  timeClient->getFormattedTime();
        root["dateD"] =  timeClient->getDay();
        root["epochTime"] =  timeClient->getEpochTime();

        time_t t = timeClient->getEpochTime();
        root["hour"] =  hour(t);
        root["minute"] =  minute(t);
        root["second"] =  second(t);
        root["day"] =  day(t);
        root["weekday"] =  weekday(t);
        root["month"] =  month(t);
        root["year"] =  year(t);


        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

};

void ITime::loop() {
        timeClient->update();
};
