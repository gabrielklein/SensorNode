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

        if (isClientMode) {

            #ifdef TIME_ENABLED
                this->iTime = new ITime(&this->fileServ);
                this->iTime->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->iTime);
            #endif

            #ifdef TEMP_ENABLE
                this->temp = new Temp();
                this->temp->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->temp);
            #endif

            #ifdef RELAY_ENABLE
                this->relay = new Relay();
                this->relay->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->relay);
            #endif

            #ifdef SWITCH_ENABLE
                this->switc = new Switch();
                this->switc->setup();
                if (this->webServerSN != NULL)
                        this->webServerSN->addServ(this->switc);
            #endif




        }


};


/**
 * Loop
 */
void Hub::loop() {

        if (this->webServerSN!=NULL)
                this->webServerSN->loop();

        if (this->iTime!=NULL)
                this->iTime->loop();

        if (isClientMode) {
                if (this->led!=NULL)
                        this->led->loop();
        }

        if (this->temp!=NULL)
                this->temp->loop();

        if (this->apStaClient!=NULL)
                this->apStaClient->loop();

        if (this->relay!=NULL)
                this->relay->loop();

        if (this->switc!=NULL)
                this->switc->loop();

}
