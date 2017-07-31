#include "APStaClient.h"

APStaClient::APStaClient(FileServ *fileServ, Led *led) {
        this->led = led;
        this->keyStore.setup("WirelessClient", fileServ);
        this->fileServ = fileServ;
};

APStaClient::~APStaClient() {
};

/**
 * Called at setup time. Use this call to initialize some data.
 */
bool APStaClient::setup() {


        this->led->rgb(0, 50, 0, true);
        this->led->buildinLed(false);

        // Try to connect as a client
        bool success = this->connectClient();

        // If if fails, connect as an access point
        if (success) {
                return true;
        }
        else {
                this->connectAP();
                return false;
        }

};


/**
 * Get shortname used to access it using the web server.
 */
String APStaClient::servName() {
        return "ap";
}

/**
 * Register server services
 */
void APStaClient::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/ap/scan", HTTP_GET, [&] () {
                this->servScan();
        });
        webServer->on("/ap/config", HTTP_GET, [&] () {
                this->keyStore.servConfig(this->webServer);
        });
        webServer->on("/ap/reset", HTTP_GET, [&] () {
                this->servReset();
        });
        webServer->on("/ap/reboot", HTTP_GET, [&] () {
                this->servReboot();
        });

        //Android captive portal. Maybe not needed. Might be handled by notFound handler
        //webServer->on("/generate_204", HTTP_GET, [&] () {
        //        this->serv??????();
        //});

        //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
        //webServer->on("/fwlink", HTTP_GET, [&] () {
        //        this->serv??????();
        //});



}

/**
 * Reset and clear all configuration
 */
void APStaClient::servReset() {
        ServerUtil::sendSuccess(this->webServer);
        led->rgb(50, 0, 0, false);
        fileServ->deleteConfig();
        delay(500);
        ESP.reset();
};

/**
 * Just reboot
 */
void APStaClient::servReboot() {
        ServerUtil::sendSuccess(this->webServer);
        led->rgb(50, 0, 0, false);
        delay(500);
        ESP.reset();
};

/**
 * Scan wireless network
 */
void APStaClient::servScan() {
        int n = WiFi.scanNetworks();
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.createObject();
        JsonArray& networks = json.createNestedArray("ap");
        for(int i=0; i<n; i++) {
                JsonObject& data = networks.createNestedObject();
                data["ssid"] = WiFi.SSID(i);
                String e = "";
                e = e + WiFi.encryptionType(i);
                data["enc"] = e;
                data["dbm"] = WiFi.RSSI(i);
                data["bssid"] = WiFi.BSSIDstr(i);
                data["channel"] = WiFi.channel(i);
                data["isHidden"] = WiFi.isHidden(i);
        }
        //String root;
        String s;
        json.printTo(s);
        this->webServer->send(200, "application/json", s);

};


/**
 * Reconnect as client if connection if down. Don't try to reconnect as AP for a
 * security reason.
 */
void APStaClient::loop() {

        if (this->isAPEnabled)  {
                this->dnsServer.processNextRequest();
        }
        else {

        }

};

/**
 * Try to connect as a client
 * Only call it from "setup!"
 */
bool APStaClient::connectClient() {

        Serial.println("APServerClient: ConnectClient");

        //WiFi.mode(WIFI_STA);
        //        WiFi.printDiag(Serial);
        WiFi.setAutoReconnect(true);
        WiFi.mode(WIFI_STA);

        String useDHCPS;
        IPAddress ip;
        IPAddress subnet;
        IPAddress gateway;
        IPAddress dns;

        useDHCPS = this->keyStore.getValue("ip_dhcp");
        ip.fromString(this->keyStore.getValue("ip_ip"));
        subnet.fromString(this->keyStore.getValue("ip_subnet"));
        gateway.fromString(this->keyStore.getValue("ip_gateway"));
        dns.fromString(this->keyStore.getValue("ip_dns"));

        bool useDHCP = useDHCPS.equals("") || useDHCPS.toInt() == 1;

        String ssidS = this->keyStore.getValue("wireless_ssid");
        String passwordS = this->keyStore.getValue("wireless_password");

        if (ssidS.length()==0) {
                Serial.println("APServerClient: No SSID defined...");
                return false;
        }


        char ssid[ssidS.length()+1];
        char password[passwordS.length()+1];

        ssidS.toCharArray(ssid, ssidS.length()+1);
        passwordS.toCharArray(password, passwordS.length()+1);

        // WL_IDLE_STATUS = 0;
        //WL_NO_SSID_AVAIL = 1; <- No SSID found
        //WL_CONNECTED = 3;
        //WL_CONNECT_FAILED = 4;
        //WL_DISCONNECTED = 6 <- At the beginning, or wrong password
        //  WL_NO_SHIELD = 255

        unsigned long time = millis();

        //WiFi.config(ip, gateway, subnet, dns);
        //WiFi.begin (ssid, password);
        WiFi.begin (ssid, password);

        while ( WiFi.status() != WL_CONNECTED ) {
                for (int i=0; i<2; i++) {
                        delay ( 75 );
                        this->led->rgb(0, 0, 50, true);
                        this->led->buildinLed(true);
                        delay ( 75 );
                        this->led->rgb(0, 0, 0, true);
                        this->led->buildinLed(false);
                }
                Serial.println("APServerClient: Connecting...");
                unsigned long timeN = millis();
                // Timeout after 5 seconds
                if (timeN - time > 10000 || WiFi.status() == WL_NO_SSID_AVAIL) {
                        Serial.println("APServerClient: Timeout with no connection...");
                        this->led->rgb(50, 0, 0, true);
                        return false;
                }
        }
        this->led->buildinLed(false);


        // Set static ip if needed
        if ( ip != 0 && !useDHCP ) {
                if ( dns != 0 ) {
                        if (!WiFi.config(ip, gateway, subnet, dns)) {
                                Serial.println("Problems configuring WiFi access (with dns)");
                        }
                }
                else {
                        if (!WiFi.config(ip, gateway, subnet)) {
                                Serial.println("Problems configuring WiFi access");
                        }
                }
        }

        IPAddress myIP = WiFi.localIP();
        Serial.println("APServerClient: Connected as client: ");
        Serial.println(myIP);

        // TODO: Check that we have access to server
        Serial.println("************ TODO Check access to server");

        // TODO: Reset after a few minutes if in AP mode, no web access
        // (Try to go in client mode.)
        Serial.println("************ TODO Reset if in AP mode after a few minutes.");


        this->led->rgb(0, 10, 0, true);
        this->led->buildinLed(true);

        return true;
}

/**
 * Try to connect as a server
 * Only call it from "setup!"
 */
bool APStaClient::connectAP() {

        Serial.println("APServerClient: ConnectAP");

        uint8_t mac[WL_MAC_ADDR_LENGTH];
        WiFi.softAPmacAddress(mac);

        IPAddress addr(192, 168, 0, 1);

        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(addr, addr, IPAddress(255, 255, 255, 0));
        String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                       String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
        macID.toUpperCase();
        String AP_NameString = "SensorNode_" + macID;

        char AP_NameChar[AP_NameString.length() + 1];
        memset(AP_NameChar, 0, AP_NameString.length() + 1);

        for (int i = 0; i < AP_NameString.length(); i++)
                AP_NameChar[i] = AP_NameString.charAt(i);

        // Create the access point on channel 7 with no password.
        WiFi.softAP(AP_NameChar, NULL, 7);
        this->dnsServer.start(53, "*", addr);

        // Start MDNS
        String mDNSName = "SensorNode-" + macID;

        char mDNSNameChar[mDNSName.length() + 1];
        memset(mDNSNameChar, 0, mDNSName.length() + 1);

        for (int i = 0; i < mDNSName.length(); i++)
                mDNSNameChar[i] = mDNSName.charAt(i);

        if (MDNS.begin(mDNSNameChar)) {
                MDNS.addService("http", "tcp", 80);
                Serial.println("APServerClient: mDNS, respond to name SensorNode: " + mDNSName);
        }
        else {
                Serial.println("APServerClient: Error setting up MDNS responder!");
        }

        this->isAPEnabled = true;
        //WiFi.printDiag(Serial);

        this->led->rgb(0, 50, 50, true);
        this->led->buildinLed(true);

        return true;
}
