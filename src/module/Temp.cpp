#include "Temp.h"

Temp::Temp() {
        this->oneWire = new OneWire(TEMP_DALLAS_PIN);
        this->sensors = new DallasTemperature(this->oneWire);
        this->dht = new DHT(TEMP_SECONDARY_PIN, TEMP_SECONDARY_TYPE);
};

Temp::~Temp() {
        delete(this->sensors);
        delete(this->oneWire);
        delete(this->dht);
};


void Temp::setup() {

        this->sensors->begin();
        dht->begin();

        this->deviceCount = this->sensors->getDeviceCount();
        if (this->deviceCount>TEMP_DALLAS_MAX_MODULE) {
                Serial.println("You have more temperature sensors than accepted. Change DallasTemp.h");
                this->deviceCount = TEMP_DALLAS_MAX_MODULE;
        }

        for (int i=0; i<this->deviceCount; i++) {
                this->sensors->getAddress(this->addr[i], i);
        }

};


String Temp::servName() {
        return "temperature";
};

/**
 * Refresh temperature in the loop phase
 */
void Temp::loop() {

        long d = millis()-timeLastUpdate;
        if (d>=0 && d<TEMP_UPDATE_EVERY_MS) {
                return;
        }

        timeLastUpdate = millis();
        dhtHumidity = dht->readHumidity();
        dhtTemp = dht->readTemperature();

        for (int i=0; i<this->deviceCount; i++) {
                this->sensors->requestTemperaturesByAddress(this->addr[i]);
        }


};

void Temp::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/temperature/get", HTTP_GET, [&] () {this->servTemp(); });
};

/**
 * Write temperature in a json string
 */
void Temp::servTemp() {

        bool doRefresh  = false;
        int currentArgCount = this->webServer->args();
        for (int i = 0; i < currentArgCount; ++i) {
                String key = this->webServer->argName(i);
                String value = this->webServer->arg(i);
                // /temperature/get?forceRefresh=1
                if (key.equals("forceRefresh")) {
                        doRefresh = value.equals("1");
                }
        }


        long start = millis();

        if (doRefresh) {
                timeLastUpdate = 0;
                loop();
        }

        char hexC[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
        float temp = this->sensors->getTempCByIndex(0);

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        JsonArray& tempJ = root.createNestedArray("temp");

        {
                if (!isnan(dhtTemp) && !isnan(dhtHumidity)) {
                        JsonObject& x = tempJ.createNestedObject();
                        x["model"] = "DHT22";
                        x["addr"] = TEMP_SECONDARY_NAME;
                        x["humidity"] = dhtHumidity;
                        x["tempC"] = dhtTemp;
                        x["tempF"] = DallasTemperature::toFahrenheit(dhtTemp);
                }
        }

        for (int i=0; i<this->deviceCount; i++) {
                JsonObject& x = tempJ.createNestedObject();
                //this->sensors->requestTemperaturesByAddress(this->addr[i]);
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
