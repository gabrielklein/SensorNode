#include "ITime.h"

ITime::ITime(FileServ *fileServ) {
        this->keyStore.setup("ITime", fileServ);
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
        webServer->on("/time/get", HTTP_GET, [&] () {
                this->servTime();
        });
        webServer->on("/time/timezone", HTTP_GET, [&] () {
                this->servTimezone();
        });
        webServer->on("/time/config", HTTP_GET, [&] () {
                if (this->keyStore.servConfig(this->webServer)) {
                        tzCurrentIndex=-1;
                }
        });
};

/**
 * Get current time
 */
void ITime::servTime() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();

        JsonObject& utc = root.createNestedObject("utc");

        time_t t = timeClient->getEpochTime();

        root["timeUnix"] = t;

        utc["timezone"] =  "UTC";
        utc["epochTime"] =  timeClient->getEpochTime();
        utc["hms"] =  getFormattedTime(t);
        //utc["date"] =  timeClient->getDay();
        utc["hour"] =  hour(t);
        utc["minute"] =  minute(t);
        utc["second"] =  second(t);
        utc["day"] =  day(t);
        utc["weekday"] =  weekday(t);
        utc["month"] =  month(t);
        utc["year"] =  year(t);


        JsonObject& local = root.createNestedObject("local");
        int index = getTimezoneIndex();
        Timezone *tzz = tz.tz[index];
        String name = tz.name[index];

        TimeChangeRule *tcr;
        time_t t2 = tzz->toLocal(t, &tcr);
        local["timezone"] =  name;
        local["hms"] =  getFormattedTime(t2);
        //local["date"] =  timeClient->getDay();
        local["hour"] =  hour(t2);
        local["minute"] =  minute(t2);
        local["second"] =  second(t2);
        local["day"] =  day(t2);
        local["weekday"] =  weekday(t2);
        local["month"] =  month(t2);
        local["year"] =  year(t2);
        local["tcr"] = tcr->abbrev;


        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

};

/**
 * Get list of supported timezones.
 */
void ITime::servTimezone() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        JsonArray& tzo = root.createNestedArray("tz");

        time_t utc = timeClient->getEpochTime();
        TimeChangeRule *tcr;
        for (int i=0; i<this->tz.tzCount; i++) {
                JsonObject& x = tzo.createNestedObject();
                time_t t = tz.tz[i]->toLocal(utc, &tcr);
                x["id"] = tz.name[i];
                x["label"] = tz.label[i];
                x["tcr"] = tcr->abbrev;

                x["hms"] =  getFormattedTime(t);
                x["hour"] =  hour(t);
                x["minute"] =  minute(t);
                x["second"] =  second(t);
        }

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);
};

/**
 * Get current timezone
 */
int ITime::getTimezoneIndex() {

        if (tzCurrentIndex>=0)
                return tzCurrentIndex;

        String tzK = keyStore.getValue("timezone");
        if (tzK.length() == 0) {
                tzCurrentIndex=0;
                return 0;
        }
        for (int i=0; i<this->tz.tzCount; i++) {
                if (tz.name[i].equals(tzK)) {
                        tzCurrentIndex=i;
                        return i;
                }
        }
        // Return default one
        tzCurrentIndex=0;
        return 0;
}

/**
 * Get formatted time
 */
String ITime::getFormattedTime(unsigned long rawTime) {
        unsigned long hours = (rawTime % 86400L) / 3600;
        String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

        unsigned long minutes = (rawTime % 3600) / 60;
        String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

        unsigned long seconds = rawTime % 60;
        String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

        return hoursStr + ":" + minuteStr + ":" + secondStr;
}

void ITime::loop() {
        timeClient->update();
};
