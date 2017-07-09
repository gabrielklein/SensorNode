#include "Geiger.h"


// Used for interupts
Geiger *geigerForInterupt = NULL;

Geiger::Geiger(FileServ *fileServ) {
        this->keyStore.setup("Geiger", fileServ);
};

Geiger::~Geiger() {
        if (geigerForInterupt!=NULL) {
                detachInterrupt(digitalPinToInterrupt(GEIGER_PIN));
                geigerForInterupt = NULL;
        }
};

/**
 * Called at setup time. Use this call to initialize some data.
 */
void Geiger::setup() {
        pinMode(GEIGER_PIN, INPUT);
        // Enable interrupts (in case they were previously disabled)
        interrupts();
        geigerForInterupt = this;
        attachInterrupt(digitalPinToInterrupt(GEIGER_PIN), [&]() {
                // Doesn't work :(
                //this->tubeImpulse();
                // So we do instead
                geigerForInterupt->tubeImpulse();
        }, FALLING);

        for (int i=0; i<MAX_STORE_DATA_GEIGER; i++) {
                coutGeigerPerSecond[i] = 0;
        }

        startMillis = millis();
        int t = (int) ((startMillis / 1000)%MAX_STORE_DATA_GEIGER);
        currentCountGeigerPointer = t;
}

/**
 * Impulse received on the tube
 */
void Geiger::tubeImpulse() {
        int t = (int) ((millis() / 1000)%MAX_STORE_DATA_GEIGER);

        if (currentCountGeigerPointer<t) {
                for (int i = currentCountGeigerPointer+1; i<=t; i++) {
                        coutGeigerPerSecond[i] = 0;
                }
        }

        if (currentCountGeigerPointer>t) {
                for (int i = 0; i<=t; i++) {
                        coutGeigerPerSecond[i] = 0;
                }
                for (int i = currentCountGeigerPointer+1; i<MAX_STORE_DATA_GEIGER; i++) {
                        coutGeigerPerSecond[i] = 0;
                }
        }

        coutGeigerPerSecond[t]++;
        currentCountGeigerPointer = t;
}

/**
 * Get shortname used to access it using the web server.
 */
String Geiger::servName() {
        return "geiger";
}

/**
 * Register services on this webserver
 */
void Geiger::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/geiger/config", HTTP_GET, [&] () {
                this->keyStore.servConfig(this->webServer);
        });
        webServer->on("/geiger/get", HTTP_GET, [&] () {
                this->servGet();
        });

}


/**
 * Get current led configuration
 */
void Geiger::servGet() {

        // coutGeigerPerSecond[i]
        int count5 = 0;
        int count15 = 0;
        int count60 = 0;
        int count300 = 0;

        int t = (int) ((millis() / 1000)%MAX_STORE_DATA_GEIGER);

        // We have moved this amount of seconds.
        // But not updated because we have received no data (no interupts)
        int dt = t-currentCountGeigerPointer;
        if (dt<0)
                dt+=MAX_STORE_DATA_GEIGER;

        for (int i=currentCountGeigerPointer-5+dt; i<currentCountGeigerPointer; i++) {
                if (i<0) {
                        count5+=coutGeigerPerSecond[i+MAX_STORE_DATA_GEIGER];
                }
                else {
                        count5+=coutGeigerPerSecond[i];
                }
        }
        for (int i=currentCountGeigerPointer-15+dt; i<currentCountGeigerPointer; i++) {
                if (i<0) {
                        count15+=coutGeigerPerSecond[i+MAX_STORE_DATA_GEIGER];
                }
                else {
                        count15+=coutGeigerPerSecond[i];
                }
        }
        for (int i=currentCountGeigerPointer-60+dt; i<currentCountGeigerPointer; i++) {
                if (i<0) {
                        count60+=coutGeigerPerSecond[i+MAX_STORE_DATA_GEIGER];
                }
                else {
                        count60+=coutGeigerPerSecond[i];
                }
        }
        for (int i=currentCountGeigerPointer-300+dt; i<currentCountGeigerPointer; i++) {
                if (i<0) {
                        count300+=coutGeigerPerSecond[i+MAX_STORE_DATA_GEIGER];
                }
                else {
                        count300+=coutGeigerPerSecond[i];
                }
        }

        float count5f = count5;
        float count15f = count15;
        float count60f = count60;
        float count300f = count300;

        double durMili = (millis() - startMillis)/1000.0;


        if (durMili>0.0 && durMili<5.0) {
                count5f=count5f/durMili;
        }
        else {
                count5f=count5f/5.0;
        }

        if (durMili>0.0 && durMili<15.0) {
                count15f=count15f/durMili;
        }
        else {
                count15f=count15f/15.0;
        }

        if (durMili>0.0 && durMili<60.0) {
                count60f=count60f/durMili;
        }
        else {
                count60f=count60f/60.0;
        }

        if (durMili>0.0 && durMili<300.0) {
                count300f=count300f/durMili;
        }
        else {
                count300f=count300f/300.0;
        }

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        root["pointerLastPulse"] = currentCountGeigerPointer;
        root["pointer"] = t;
        JsonArray& g = root.createNestedArray("geiger");

        JsonObject& x5 = g.createNestedObject();
        x5["duration"] = 5;
        x5["count"] = count5;
        x5["countps"] = count5f;
        x5["uSv"] = hitPerSecsTouSv(count5f);


        JsonObject& x15 = g.createNestedObject();
        x15["duration"] = 15;
        x15["count"] = count15;
        x15["countps"] = count15f;
        x15["uSv"] = hitPerSecsTouSv(count15f);

        JsonObject& x60 = g.createNestedObject();
        x60["duration"] = 60;
        x60["count"] = count60;
        x60["countps"] = count60f;
        x60["uSv"] = hitPerSecsTouSv(count60f);

        JsonObject& x300 = g.createNestedObject();
        x300["duration"] = 300;
        x300["count"] = count300;
        x300["countps"] = count300f;
        x300["uSv"] = hitPerSecsTouSv(count300f);


        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

}

/**
 * Transform hits per seconds into uSv.
 */
double Geiger::hitPerSecsTouSv(double hitPerSeconds) {
        return hitPerSeconds * 60 * conversionFactor;
}

/**
 * Do actions in the background
 */
void Geiger::loop() {


}
