#include "APStaClient.h"

APStaClient::APStaClient(FileServ *fileServ, WS281xStrip *ledStrip) {
        this->ledStrip = ledStrip;
        this->keyStore.setup("WirelessClient", fileServ);
};

APStaClient::~APStaClient() {
};

/**
 * Called at setup time. Use this call to initialize some data.
 */
void APStaClient::setup() {


        this->ledStrip->rgb(0, 50, 0);

        // Try to connect as a client
        bool success = this->connectClient();

        // If if fails, connect as an access point
        if (!success) {
                this->connectAP();
        }

};


/**
 * Get shortname used to access it using the web server.
 */
String APStaClient::servName() {
        return "ap";
}

/**
 * Register services on this webserver
 */
void APStaClient::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/ap/scan", HTTP_GET, [&] () {this->servScan(); });
        webServer->on("/ap/config", HTTP_GET, [&] () {this->keyStore.servConfig(this->webServer); });

}

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
                        this->ledStrip->rgb(0, 0, 50);
                        delay ( 75 );
                        this->ledStrip->rgb(0, 0, 0);
                }
                Serial.println("APServerClient: Connecting...");
                unsigned long timeN = millis();
                // Timeout after 5 seconds
                if (timeN - time > 10000 || WiFi.status() == WL_NO_SSID_AVAIL) {
                        Serial.println("APServerClient: Timeout with no connection...");
                        this->ledStrip->rgb(50, 0, 0);
                        return false;
                }
        }


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


        this->ledStrip->rgb(0, 50, 0);
        return true;
}

/**
 * Try to connect as a server
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

        this->ledStrip->rgb(0, 50, 50);

        return true;
}
