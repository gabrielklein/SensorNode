#include "DallasTemp.h"




DallasTemp::DallasTemp() {
        this->oneWire = new OneWire(DALLAS_PIN);
        this->sensors = new DallasTemperature(this->oneWire);
};

DallasTemp::~DallasTemp() {
        delete(this->sensors);
        delete(this->oneWire);

};


void DallasTemp::setup() {
        this->sensors->begin();

        this->deviceCount = this->sensors->getDeviceCount();
        if (this->deviceCount>DALLASTEMP_MAX_MODULE) {
                Serial.println("You have more temperature sensors than accepted. Change DallasTemp.h");
                this->deviceCount = 10;
        }

        for (int i=0; i<this->deviceCount; i++) {
                this->sensors->getAddress(this->addr[i], i);
        }

};


String DallasTemp::servName() {
        return "dallas";
};

void DallasTemp::loop() {

};

void DallasTemp::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/dallas/temp", HTTP_GET, [&] () {this->servTemp(); });
};


void DallasTemp::servTemp() {

        char hexC[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
        float temp = this->sensors->getTempCByIndex(0);

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        JsonArray& tempJ = root.createNestedArray("temp");

        long start = millis();
        for (int i=0; i<this->deviceCount; i++) {
                JsonObject& x = tempJ.createNestedObject();
                this->sensors->requestTemperaturesByAddress(this->addr[i]);
                float tt = this->sensors->getTempC(this->addr[i]);
                String mType = "Unknown";
                switch(addr[i][0]) {
                // also DS1820
                case DS18S20MODEL:
                        mType = "DS18S20";
                        break;
                case DS18B20MODEL:
                        mType = "DS18B20";
                        break;
                case DS1822MODEL:
                        mType = "DS1822";
                        break;
                case DS1825MODEL:
                        mType = "DS1825";
                        break;
                        //case DS28EA00MODEL:
                        //        mType = "DS28EA";
                        //        break;
                };


                String s;
                for(int j = 0; j < 8; j++) {
                        byte b = this->addr[i][j];
                        s += hexC[ ( b & 0xF0 ) >> 4 ];
                        s += hexC[ ( b & 0x0F ) >> 0 ];

                }

                x["model"] = mType;
                x["addr"] = s;
                x["tempC"] = tt;
                x["tempF"] = DallasTemperature::toFahrenheit(tt);
        }
        long end = millis();

        root["requestDuration"] = end-start;

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);
};
