#include "WebServerSN.h"
#include "ServerUtil.h"
#include "../Settings.h"

/**
 * Construct the webserver
 */
WebServerSN::WebServerSN(FileServ *fileServ) {
        this->webServer = new ESP8266WebServer(80);
        this->fileServ = fileServ;
}

/**
 * Destroy the webserver
 */
WebServerSN::~WebServerSN() {
        delete(this->webServer);
}

/**
 * Launch the server (call it in the setup phase!)
 */
void WebServerSN::setup() {
        Serial.println("WebServerSN: Starting webserver");
        this->webServer->onNotFound([&] () {
                this->servFiles();
        });
        this->webServer->begin();
}

/**
 * Get shortname used to access it using the web server.
 */
String WebServerSN::servName() {
        return "server";
}

void WebServerSN::apModeOnly() {
        Serial.println("WebServerSN: apModeOnly");
        apModeEnableOnly = true;
}

/**
 * Register services on this webserver
 */
void WebServerSN::servRegister(ESP8266WebServer *webServer) {
        //webServer->on("/ap/scan.do", HTTP_GET, [&] () {this->getScan(); });
}

void WebServerSN::addServ(IServer *iServer) {
        if (iServer == NULL) {
                return;
        }
        Serial.print("WebServerSN: Adding service ");
        Serial.println(iServer->servName());
        iServer->servRegister(this->webServer);
}

/**
 * Loop the server (call it in the loop phase!)
 */
void WebServerSN::loop() {
        this->webServer->handleClient();
};

/**
 * Send a response and write an header that will not be stored in cache.
 */
void WebServerSN::send(int responseCode, String content, String mime) {

        // Open the file and send the content
        String name(this->webServer->uri());

        // Avoid caching
        this->webServer->sendHeader("Expires","Sun, 01 Jan 2017 00:00:00 GMT");
        this->webServer->sendHeader("Cache-Control","no-store, no-cache, must-revalidate");
        this->webServer->sendHeader("Pragma","no-cache");

        this->webServer->setContentLength(content.length());
        this->webServer->send(responseCode, mime, content);
};

/*
 * Serve files that are stored locally.
 * Please note that all files should be in lower case.
 * Please note that only alphanumeric, digital, ., /, _ and - are accepted to avoid risks of hack
 */
void WebServerSN::servFiles() {

        // Open the file and send the content
        String name(this->webServer->uri());
        if (!name.equals("/favicon.ico")) {
                Serial.print("WebServerSN: Serv file: ");
                Serial.println(name);
        }

        if (name.equals("/")) {
                name += "index.html";
        }

        String nameLC = String(name);
        nameLC.toLowerCase();

        #ifdef WEB_SERVER_EXPOSE_CONFIG

        if (nameLC.equals("/files")) {

                DynamicJsonBuffer jsonBuffer;
                JsonObject& root = jsonBuffer.createObject();
                JsonArray& g = root.createNestedArray("files");

                Dir dir = SPIFFS.openDir("/");
                while (dir.next()) {
                        JsonObject& x5 = g.createNestedObject();
                        x5["file"] = dir.fileName();
                        x5["size"] = dir.fileSize();
                }

                String s;
                root.printTo(s);
                webServer->send(200, "application/json", s);
                return;
        }

        #endif

        // Don't allow /config/ in the name
        #ifndef WEB_SERVER_EXPOSE_CONFIG
        if (nameLC.indexOf("/config/")>=0 || nameLC.indexOf("..")>=0) {
                Serial.println("WebServerSN: Access forbidden to /config/!");
                String s2 = "<html><head><title>No access</title></head><body>No access here!</body></html>";
                this->send(403, s2, "text/html");
                return;
        }
        #endif

        if (apModeEnableOnly) {
                if (name.startsWith("/index-se.html")) {
                        Serial.println("WebServerSN: No access to /index-se.html!");
                        String s2 = "<html><head><meta http-equiv=\"refresh\" content=\"5;URL='/ap/index.html'\" /><title>No access to /index-se.html when in ap mode</title></head><body>No access to /index-se.html when in ap mode.<br />Configure access point <a href=\"/index-se.html\">here</a>.</body></html>";
                        this->send(403, s2, "text/html");
                        return;
                }

                if (name.startsWith("/index.html")) {
                        Serial.println("WebServerSN: Redirect /index-ap.html to ap!");
                        String s2 = "<html><head><meta http-equiv=\"refresh\" content=\"5;URL='/index-ap.html'\" /><title>Redirect to ap mode</title></head><body>Redirect to AP.<br />Configure access point <a href=\"/index-ap.html\">here</a>.</body></html>";
                        this->send(403, s2, "text/html");
                        return;
                }
        }

        // Check the chars in the name
        for (int i=0; i<name.length(); i++) {
                char c = name.charAt(i);
                if (isAlphaNumeric(c) || c == '.' || c == '/'|| c == '-' || c == '_'|| isDigit(c)) {
                        // Success
                }
                else {
                        Serial.println("WebServerSN: Forbidden access to this file!");
                        String s2 = "<html><head><title>No access</title></head><body>Forbidden char in the name!</body></html>";
                        this->send(403, s2, "text/html");
                        return;
                }
        }

        // Open Files
        File webFile = this->fileServ->openR(name);

        // We must redirect on the ip if we have typed another site.
        //if (!webFile || !this->webServer->hostHeader().equals(this->ip)) {

        //request->redirect("/login");
        //      String str = String();
        //    str += "<!DOCTYPE html><html><head><title>Redirecting to SensorNode</title></head><body>"
        //             "<h1>Redirecting to SensorNode</h1><p><a href=\"http://";
        //      str += this->ip;
        //      str += "/ap/index.html\">  Click to redirect now</a><script>document.location.href='http://";
        //      str += this->ip;
        //      str +="/ap/index.html';</script>";
        //      str += "</p></body></html>";
        //      this->send(200, str, "text/html");
        //      this->notif->processing();
        //      return;
        //}

        if (webFile) {
                String mime = ServerUtil::getMime(name);
                this->webServer->streamFile(webFile, mime);
                webFile.close();
                return;
        }


        Serial.print("WebServerSN: File not found! ");
        Serial.println(name);
        String s2 = "<html><head><title>File not found</title></head><body>Sorry this file does not exists.</body></html>";
        this->send(404, s2, "text/html");

};
