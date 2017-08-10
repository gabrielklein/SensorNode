#include "Hub.h"
#include "../Settings.h"

Hub::Hub() {
};
Hub::~Hub() {
        delete(this->webServerSN);
        delete(this->apStaClient);
        delete(this->led);
        delete(this->temp);
        delete(this->relay);
        delete(this->switc);
        delete(this->geiger);
        delete(this->mqtt);
        delete(this->otaUpdate);
};

/**
 * Called at setup time. Use this call to initialize some data.
 */
void Hub::setup() {

        // Use serial
        Serial.begin ( 115200 );
        Serial.println ("\nStarting Hub");

        this->fileServ.setup();

        #ifdef WEB_SERVER_SN_ENABLE
        this->webServerSN = new WebServerSN(&this->fileServ);
        this->webServerSN->addServ(this->webServerSN);
        this->webServerSN->setup();
        #endif

        #ifdef WS281X_STRIP_ENABLE
        this->led = new Led(&this->fileServ);
        this->led->setup();
        this->led->rgb(0, 0, 0, 50);
        if (this->webServerSN != NULL)
                this->webServerSN->addServ(this->led);
        #endif

        #ifdef AP_SERVER_CLIENT_ENABLE
        this->apStaClient = new APStaClient(&this->fileServ, this->led);
        isClientMode = this->apStaClient->setup();
        if (this->webServerSN != NULL) {
                this->webServerSN->addServ(this->apStaClient);
                if (!isClientMode) {
                        this->webServerSN->apModeOnly();
                }
        }
        #endif

        #ifdef OTA_ENABLE
        this->otaUpdate = new OTAUpdate();
        this->otaUpdate->setup();
        #endif


        if (isClientMode) {

            #ifdef TIME_ENABLE
                this->iTime = new ITime(&this->fileServ);
                this->iTime->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->iTime);
            #endif

            #ifdef MQTT_ENABLE
                this->mqtt = new MQTT(&this->fileServ);
                this->mqtt->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->mqtt);
            #endif

            #ifdef TEMP_ENABLE
                this->temp = new Temp();
                this->temp->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->temp);
                if (this->mqtt != NULL)
                        this->mqtt->addServ(this->temp);
            #endif

            #ifdef RELAY_ENABLE
                this->relay = new Relay();
                this->relay->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->relay);
                if (this->mqtt != NULL)
                        this->mqtt->addServ(this->relay);
            #endif

            #ifdef SWITCH_ENABLE
                this->switc = new Switch();
                this->switc->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->switc);
                if (this->mqtt != NULL)
                        this->mqtt->addServ(this->switc);
            #endif

            #ifdef GEIGER_ENABLE
                this->geiger = new Geiger(&this->fileServ);
                this->geiger->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->geiger);
                if (this->mqtt != NULL)
                        this->mqtt->addServ(this->geiger);
            #endif






        }


};


/**
 * Loop
 */
void Hub::loop() {

        if (this->webServerSN!=NULL) {
                unsigned long now = millis();
                this->webServerSN->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for WebServerSN is a bit long: ");
                        Serial.println(duration);
                }
        }

        if (this->otaUpdate!=NULL) {
                unsigned long now = millis();
                this->otaUpdate->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for otaUpdate is a bit long: ");
                        Serial.println(duration);
                }
        }

        if (!isClientMode) {
                return;
        }

        if (this->iTime!=NULL) {
                unsigned long now = millis();
                this->iTime->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for iTime is a bit long: ");
                        Serial.println(duration);
                }
        }

        if (this->led!=NULL) {
                unsigned long now = millis();
                this->led->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for led is a bit long: ");
                        Serial.println(duration);
                }
        }

        if (this->temp!=NULL) {
                unsigned long now = millis();
                this->temp->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for temp is a bit long: ");
                        Serial.println(duration);
                }
        }

        if (this->apStaClient!=NULL) {
                unsigned long now = millis();
                this->apStaClient->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for apStaClient is a bit long: ");
                        Serial.println(duration);
                }
        }

        if (this->relay!=NULL) {
                unsigned long now = millis();
                this->relay->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for relay is a bit long: ");
                        Serial.println(duration);
                }
        }

        if (this->switc!=NULL) {
                unsigned long now = millis();
                this->switc->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for switc is a bit long: ");
                        Serial.println(duration);
                }
        }


        if (this->geiger!=NULL) {
                unsigned long now = millis();
                this->geiger->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for geiger is a bit long: ");
                        Serial.println(duration);
                }
        }


        if (this->mqtt!=NULL) {
                unsigned long now = millis();
                this->mqtt->loop();
                unsigned long duration = millis() - now;
                if (duration > 100) {
                        Serial.print("Loop for mqtt is a bit long: ");
                        Serial.println(duration);
                }
        }



}
