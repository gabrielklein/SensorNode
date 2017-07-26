#include "MQTT.h"

MQTT::MQTT(FileServ *fileServ) {

        for (int i=0; i<MAX_IMQTT; i++) {
                this->imqtt[i] = NULL;
        }

        this->keyStore.setup("MQTT", fileServ);
}

MQTT::~MQTT() {
        if (client) {
                client->disconnect();
                delete(client);
                client = NULL;
        }
        delete(espClient);
}

void MQTT::setup() {


        topicChannel = this->keyStore.getValue("mqtt_topic_message");
        enable = this->keyStore.getValueBool("mqtt_enable");

        if (client) {
                client->disconnect();
                delete(client);
                client = NULL;
        }

        if (espClient) {
                delete(espClient);
                espClient = NULL;
        }

        // Setup client
        // WARNING called when we change configuration too!
        if (enable) {


                espClient = new WiFiClient();
                client = new PubSubClient(*espClient);

                client->setServer(this->keyStore.getValue("mqtt_server").c_str(), this->keyStore.getValueInt("mqtt_port"));

                uint8_t mac[WL_MAC_ADDR_LENGTH];
                WiFi.softAPmacAddress(mac);

                macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                        String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);


                IPAddress myIP = WiFi.localIP();
                String connected;
                connected += "SensorNode ";
                connected += macID;
                connected += " ";
                connected += myIP.toString();

                if (client->connect(this->keyStore.getValue("mqtt_server").c_str(), this->keyStore.getValue("mqtt_user").c_str(), this->keyStore.getValue("mqtt_password").c_str())) {
                        Serial.print("Connected to MQTT ");
                        Serial.println(connected);

                        client->publish(this->keyStore.getValue("mqtt_topic_register").c_str(), connected.c_str(), true);
                        Serial.println(connected);

                } else {
                        Serial.print("Failed to connect to MQTT server, rc=");
                        Serial.println(client->state());
                }


        }

}

void MQTT::loop() {

        if(!enable) {
                return;
        }

        client->loop();

        for (int i=0; i<imqttCurrent; i++) {
                IMQTT *iMQTT = imqtt[i];
                String m = iMQTT->servGetMQTT();
                if (m.length()>0) {
                        String mess = macID + " " + iMQTT->servName() + " " + m;
                        // Reconnected if disconnected
                        if (!client->connected()) {
                                if (client->connect(this->keyStore.getValue("mqtt_server").c_str(), this->keyStore.getValue("mqtt_user").c_str(), this->keyStore.getValue("mqtt_password").c_str())) {
                                        //Serial.println("reconnect success");
                                }
                                else {
                                        //Serial.println("reconnect failed");
                                }
                        }
                        boolean r = client->publish(topicChannel.c_str(), mess.c_str(), true);
                        if (r) {
                                Serial.println("Send MQTT "+mess);
                        }
                        else {
                                Serial.println("Failed Send MQTT "+mess);
                        }
                        //Serial.println(r);
                }
        }

}

void MQTT::addServ(IMQTT *iMQTT) {

        if (iMQTT == NULL) {
                return;
        }

        if (this==NULL) {
                Serial.println("this is NULL!!");
        }

        if (this->imqttCurrent >= MAX_IMQTT) {
                Serial.println("WARNING, INCREASE MAX_IMQTT. Cannot add service.");
                return;
        }

        // Bug here..
        this->imqtt[this->imqttCurrent] = iMQTT;
        this->imqttCurrent = this->imqttCurrent+1;
}

String MQTT::servName() {
        return "mqtt";
};

void MQTT::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/mqtt/config", HTTP_GET, [&] () {
                this->keyStore.servConfig(this->webServer);
                this->setup();
        });
};
