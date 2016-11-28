#include "MyClient.h"


MyClient::MyClient(Config* config, Notification *notification) {
        this->config = config;
        this->notification = notification;
}


MyClient::~MyClient() {

}

void MyClient::setup() {

        notification->processing();

        // Config wifi
        String dhcp = config->getValue("ip_dhcp");
        if (dhcp.equals("0")) {

                IPAddress ip;
                IPAddress subnet;
                IPAddress gateway;
                IPAddress dns;

                ip.fromString(config->getValue("ip_ip"));
                subnet.fromString(config->getValue("ip_subnet"));
                gateway.fromString(config->getValue("ip_gateway"));
                dns.fromString(config->getValue("ip_dns"));

                WiFi.config(ip, gateway, subnet, dns);
        }


        // Start wifi
        char ssid[256];
        char password[256];
        config->getValue("wireless_ssid").toCharArray(ssid, 256);
        config->getValue("wireless_password").toCharArray(password, 256);
        WiFi.begin (ssid, password);

        int count = 0;
        while ( WiFi.status() != WL_CONNECTED ) {
                notification->processing();
                delay ( 200 );
                notification->off();
                delay ( 200 );
                if (count++ > 5*10) {
                        notification->error();
                        return;
                }
        }


        notification->ready();

}

void MyClient::loop() {

}
