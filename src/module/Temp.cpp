#include "Temp.h"

Temp::Temp() {
  #ifdef TEMP_DALLAS_ENABLE
        this->oneWire = new OneWire(TEMP_DALLAS_PIN);
        this->sensors = new DallasTemperature(this->oneWire);
        for (int i=0; i<TEMP_DALLAS_MAX_MODULE; i++) {
                lastMQTTMessDallas[i] = 0;
        }
  #endif
  #ifdef TEMP_SECONDARY_ENABLE
        lastMQTTMessDHT = 0;
        this->dht = new DHT(TEMP_SECONDARY_PIN, TEMP_SECONDARY_TYPE);
  #endif
};

Temp::~Temp() {
  #ifdef TEMP_DALLAS_ENABLE
        delete(this->sensors);
        delete(this->oneWire);
  #endif
  #ifdef TEMP_SECONDARY_ENABLE
        delete(this->dht);
  #endif



};

void Temp::servSetMQTT(String mess) {
}

#ifdef TEMP_DALLAS_ENABLE
String Temp::addr2MType(DeviceAddress* addr) {
        switch(*addr[0]) {
        // also DS1820
        case DS18S20MODEL:
                return "DS18S20";
        case DS18B20MODEL:
                return "DS18B20";
        case DS1822MODEL:
                return "DS1822";
        case DS1825MODEL:
                return "DS1825";
                //case DS28EA00MODEL:
                //        mType = "DS28EA";
                //        break;
        };

        return "Unknown";
}
#endif

#ifdef TEMP_DALLAS_ENABLE
String Temp::addr2Hex(DeviceAddress* addr) {
        String s2;
        for(int j = 0; j < 8; j++) {
                byte b = *addr[j];
                s2 += this->hexC[ ( b & 0xF0 ) >> 4 ];
                s2 += this->hexC[ ( b & 0x0F ) >> 0 ];

        }
        return s2;
}
#endif

String Temp::servGetMQTT() {

        unsigned long now = millis();


        // Send every 30 seconds, except if receive more than 10 particles in 5s
        // Then send every 5 seconds
#ifdef TEMP_SECONDARY_ENABLE
        if ((now-lastMQTTMessDHT)>30000l || now<lastMQTTMessDHT) {

                lastMQTTMessDHT = now;

                {
                        String s;
                        if (!isnan(dhtTemp) && !isnan(dhtHumidity)) {
                                s += TEMP_SECONDARY_NAME;
                                s += " ";
                                s += TEMP_SECONDARY_NAME;
                                s += " ";
                                s += String(dhtTemp, 1);
                                s += " ";
                                s += String(DallasTemperature::toFahrenheit(dhtTemp), 1);
                                s += " ";
                                s += String(dhtHumidity, 1);
                                s += " ";
                                return s;
                        }
                }
        }
#endif

#ifdef TEMP_DALLAS_ENABLE

        for (int i = 0; i<deviceCount; i++) {
                if ((now-lastMQTTMessDallas[i])>30000l || now<lastMQTTMessDallas[i]) {

                        lastMQTTMessDallas[i] = now;

                        String s;

                        //this->sensors->requestTemperaturesByAddress(this->addr[i]);
                        float tt = this->sensors->getTempC(this->addr[i]);

                        String mType = addr2MType(&this->addr[i]);
                        String s2 = addr2Hex(&this->addr[i]);

                        s += mType;
                        s += " ";
                        s += s2;
                        s += " ";
                        s += String(tt, 1);
                        s += " ";
                        s += String(DallasTemperature::toFahrenheit(tt), 1);
                        s += " ";
                        s += "-";

                        return s;

                }
        }

#endif
        //s += "5 ";
        //s += String(hitPerSecsTouSv(count5f), 4);
        return "";
}

void Temp::setup() {

#ifdef TEMP_SECONDARY_ENABLE
        dht->begin();
#endif

#ifdef TEMP_DALLAS_ENABLE
        this->sensors->begin();
        this->deviceCount = this->sensors->getDeviceCount();
        if (this->deviceCount>TEMP_DALLAS_MAX_MODULE) {
                Serial.println("You have more temperature sensors than accepted. Change DallasTemp.h");
                this->deviceCount = TEMP_DALLAS_MAX_MODULE;
        }

        for (int i=0; i<this->deviceCount; i++) {
                this->sensors->getAddress(this->addr[i], i);
        }
#endif
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

#ifdef TEMP_SECONDARY_ENABLE
        dhtHumidity = dht->readHumidity();
        dhtTemp = dht->readTemperature();
#endif

#ifdef TEMP_DALLAS_ENABLE
        for (int i=0; i<this->deviceCount; i++) {
                this->sensors->requestTemperaturesByAddress(this->addr[i]);
        }
#endif

};

void Temp::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/temperature/get", HTTP_GET, [&] () {
                this->servTemp();
        });
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

        #ifdef TEMP_DALLAS_ENABLE
        float temp = this->sensors->getTempCByIndex(0);
        #endif

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        JsonArray& tempJ = root.createNestedArray("temp");

#ifdef TEMP_SECONDARY_ENABLE
        {
                if (!isnan(dhtTemp) && !isnan(dhtHumidity)) {
                        JsonObject& x = tempJ.createNestedObject();
                        x["model"] = TEMP_SECONDARY_NAME;
                        x["addr"] = TEMP_SECONDARY_NAME;
                        x["humidity"] = dhtHumidity;
                        x["tempC"] = dhtTemp;
                        x["tempF"] = DallasTemperature::toFahrenheit(dhtTemp);
                }
        }
#endif

#ifdef TEMP_DALLAS_ENABLE
        for (int i=0; i<this->deviceCount; i++) {
                JsonObject& x = tempJ.createNestedObject();
                //this->sensors->requestTemperaturesByAddress(this->addr[i]);
                float tt = this->sensors->getTempC(this->addr[i]);

                String mType = addr2MType(&this->addr[i]);
                String s = addr2Hex(&this->addr[i]);

                x["model"] = mType;
                x["addr"] = s;
                x["tempC"] = tt;
                x["tempF"] = DallasTemperature::toFahrenheit(tt);
        }
  #endif
        long end = millis();

        root["requestDuration"] = end-start;

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);
};
