#include "MyWifiAP.h"

MyWifiAP::MyWifiAP(Config* config, Notification *notif) {
        this->webServer = new ESP8266WebServer(80);
        this->config = config;
        this->notif = notif;
}

MyWifiAP::~MyWifiAP() {
        delete(this->webServer);
}

void MyWifiAP::setup() {

        this->notif->processing();
        WiFi.mode(WIFI_AP);

        // Do a little work to get a unique-ish name. Append the
        // last two bytes of the MAC (HEX'd) to "Thing-":
        uint8_t mac[WL_MAC_ADDR_LENGTH];
        WiFi.softAPmacAddress(mac);

        IPAddress apIP(192, 168, 0, 1);

        this->ip = new char[20];
        String str = String(192);
        str += ".";
        str += String(168);
        str += ".";
        str += String(0);
        str += ".";
        str += String(1);
        str.toCharArray(this->ip, 20);

        WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
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

        // Start MDNS
        //if ( MDNS.begin ( AP_NameChar ) ) {
        //        Serial.println ( "Started MDNS" );
        //}

        // Start the DNS
        this->dnsServer.start(53, "*", apIP);

        // Start the webserver
        this->webServer->on("/ap/config.post", HTTP_POST, [&] () {this->postConfig(); });
        this->webServer->on("/ap/scan.do", HTTP_GET, [&] () {this->getScan(); });
        this->webServer->on("/ap/config.do", HTTP_GET, [&] () {this->getConfig(); });
        this->webServer->on("/ap/reboot.do", HTTP_GET, [&] () {this->doReboot(); });
        this->webServer->on("/ap/reset.do", HTTP_GET, [&] () {this->doReset(); });
        this->webServer->onNotFound([&] () {this->error(); });
        this->webServer->begin();

        this->notif->ready();
};

void MyWifiAP::getScan() {

        this->notif->processing();


        String s;
        int n = WiFi.scanNetworks();
        for (int i = 0; i < n; ++i)
        {
                s += WiFi.RSSI(i);
                s += ",";
                s += WiFi.encryptionType(i);
                s += ",";
                s += WiFi.channel(i);
                s += ",";
                s += WiFi.SSID(i);
                s += "\n";
                //obj1["bssid"] = WiFi.BSSID(i);
        }

        this->send(200, s, "plain/text");

        this->notif->ready();
};

void MyWifiAP::getConfig() {

        String s = config->getValues();
        this->send(200, s, "plain/text");

};


void MyWifiAP::doReset() {

        this->notif->error();

        String s = "reset";
        this->send(200, s, "plain/text");

        this->config->reset();
        this->notif->ready();

};

void MyWifiAP::error() {

        this->notif->processing();

        // Open the file and send the content
        String name(this->webServer->uri());

        if (name.equals("/")) {
                name+="index.html";
        }

        String nameLC = String(name);
        nameLC.toLowerCase();

        // Don't allow /config/ in the name
        if (nameLC.indexOf("/config/")>=0) {
                String s2 = "<html><head><title>No access</title></head><body>No access here!</body></html>";
                this->send(403, s2, "text/html");
                this->notif->error();
                return;
        }

        // Check the chars in the name
        for (int i=0; i<name.length(); i++) {
                char c = name.charAt(i);
                if (isAlphaNumeric(c) || c == '.' || c == '/'|| c == '-' || c == '_'|| isDigit(c)) {
                        // Success
                }
                else {
                        String s2 = "<html><head><title>No access</title></head><body>Forbidden char in the name!</body></html>";
                        this->send(403, s2, "text/html");
                        this->notif->error();
                        return;
                }
        }

        // Open Files
        File webFile = SPIFFS.open(name, "r");

        // We must redirect on the ip if we have typed another site.
        if (!webFile || !this->webServer->hostHeader().equals(this->ip)) {

                //request->redirect("/login");

                String str = String();
                str += "<!DOCTYPE html><html><head><title>Redirecting to SensorNode</title></head><body>"
                       "<h1>Redirecting to SensorNode</h1><p><a href=\"http://";
                str += this->ip;
                str += "/ap/index.html\">  Click to redirect now</a><script>document.location.href='http://";
                str += this->ip;
                str +="/ap/index.html';</script>";
                str += "</p></body></html>";
                this->send(200, str, "text/html");
                this->notif->processing();
                return;
        }

        if (webFile) {

                String mime = ServerUtil::getMime(name);
                this->webServer->streamFile(webFile, mime);
                this->notif->ready();
                return;
        }


        String s2 = "<html><head><title>File not found</title></head><body>Sorry this file does not exists.</body></html>";
        this->send(404, s2, "text/html");
        this->notif->processing();

};

void MyWifiAP::send(int responseCode, String content, String mime) {

        // Open the file and send the content
        String name(this->webServer->uri());

        // Avoid caching
        this->webServer->sendHeader("Expires","Sun, 01 Jan 2014 00:00:00 GMT");
        this->webServer->sendHeader("Cache-Control","no-store, no-cache, must-revalidate");
        this->webServer->sendHeader("Pragma","no-cache");

        this->webServer->setContentLength(content.length());
        this->webServer->send(responseCode, mime, content);

}



void MyWifiAP::postConfig() {
        String s = "success";
        this->send(200, s, "plain/text");

        int currentArgCount = this->webServer->args();
        for (int i = 0; i < currentArgCount; ++i) {
                String key = this->webServer->argName(i);
                String value = this->webServer->arg(i);
                this->config->setValue(key, value);
        }

        this->config->saveConfig();
}


void MyWifiAP::doReboot() {

        this->notif->error();

        String s = "reboot";
        this->send(200, s, "plain/text");

        delay(300);
        ESP.restart();
}


void MyWifiAP::loop() {
        this->dnsServer.processNextRequest();
        this->webServer->handleClient();
};
