#include "Services.h"
#include "../Settings.h"

Services::Services(Hub* hub) {
        this->hub = hub;
};

Services::~Services() {

};

/**
 * Called at setup time. Use this call to initialize some data.
 */
void Services::setup() {
        hub->webServerSN->addServ(this);
};


/**
 * Loop
 */
void Services::loop() {


}

/**
 * Name of this service
 */
String Services::servName() {
        return "services";
}

/**
 * Register services
 */
void Services::servRegister(ESP8266WebServer *webServer) {

        this->webServer = webServer;

        //hub->webServer->on("/services/get", HTTP_GET, [&] () {
        //        this->servGet();
        //});

        //this->webServer->on("/services/config", HTTP_GET, [&] () {
        //        keyStore.servConfig(this->webServer);
        //});

}

/**
 * Get list of services configuration
 */
void Services::servGet() {


        //String s;
        //root.printTo(s);
        //hub->webServer->send(200, "application/json", s);

}
