#include "WS281xStrip.h"

WS281xStrip::WS281xStrip() {
        this->neo = new NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod>(this->countLed, 2 /*port*/);
};

WS281xStrip::~WS281xStrip() {
        delete(this->neo);
};

/**
 * Called at setup time. Use this call to initialize some data.
 */
void WS281xStrip::setup() {
        this->neo->Begin();
        this->neo->ClearTo(RgbColor(0, 0, 0));
        this->neo->Show();

        for (int i=0; i<countLed; i++) {
                this->neo->SetPixelColor(i, RgbColor(50,0,0));
                this->neo->Show();
                delay(5);
                this->neo->SetPixelColor(i, RgbColor(0,0,0));
        }
        for (int i=0; i<countLed; i++) {
                this->neo->SetPixelColor(i, RgbColor(0,50,0));
                this->neo->Show();
                delay(5);
                this->neo->SetPixelColor(i, RgbColor(0,0,0));
        }
        for (int i=0; i<countLed; i++) {
                this->neo->SetPixelColor(i, RgbColor(0,0,50));
                this->neo->Show();
                delay(5);
                this->neo->SetPixelColor(i, RgbColor(0,0,0));
        }
        this->neo->Show();
}

/**
 * Get shortname used to access it using the web server.
 */
String WS281xStrip::servName() {
        return "led";
}

/**
 * Register services on this webserver
 */
void WS281xStrip::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/led/set", HTTP_GET, [&] () {this->servSet(); });

        //webServer->on("/ap/scan.do", HTTP_GET, [&] () {this->getScan(); });
}

/**
 * Set a led exemple: /strip/set?l=0,1&r=125&g=55&b=220
 */
void WS281xStrip::servSet() {

        String l = "";
        int r = 0;
        int g = 0;
        int b = 0;

        int currentArgCount = this->webServer->args();
        for (int i = 0; i < currentArgCount; ++i) {
                String key = this->webServer->argName(i);
                String value = this->webServer->arg(i);
                if (value == NULL) {
                        value = "";
                }

                if (key.equals("l")) {
                        l = value;
                }
                if (key.equals("r")) {
                        r = value.toInt();
                }
                if (key.equals("g")) {
                        g = value.toInt();
                }
                if (key.equals("b")) {
                        b = value.toInt();
                }
        }

        RgbColor c = RgbColor(r, g, b);

        if (l.equals("")) {
                ServerUtil::sendFail(this->webServer, 1, "l parameter is not defined");
        }
        else if (l.equals("*")) {
                for (int i=0; i<countLed; i++) {
                        this->neo->SetPixelColor(i, c);
                }
                this->neo->Show();
                ServerUtil::sendSuccess(this->webServer);
        }
        else {
                // To detect the last number
                l = l +",";
                int ls = l.length();
                int s = 0;
                for (int i = 0; i<ls; i++) {
                        char cc = l.charAt(i);
                        if (cc>='0' && cc<='9') {
                                if (s == -1)
                                        s = 0;
                                s = s*10;
                                s += (cc-'0');
                        }
                        else {
                                if (s>=0 && s<countLed) {
                                        this->neo->SetPixelColor(s, c);
                                }
                                s = -1;
                        }
                }
                this->neo->Show();
                ServerUtil::sendSuccess(this->webServer);
        }


}

/**
 * Do actions in the background
 */
void WS281xStrip::loop() {
        counter1++;
        if (counter1>100) {
                this->neo->SetPixelColor(counter2, RgbColor(0,0,0));
                counter1 = 0;
                counter2++;
                if (counter2>=countLed) {
                        counter2 = 0;
                }
                this->neo->SetPixelColor(counter2, RgbColor(0,50,0));
                this->neo->Show();
                counter1 = 0;
        }
}

/**
 * Set a rgb color (simple on-off)
 */
void WS281xStrip::rgb(int r, int g, int b) {
        this->neo->ClearTo(RgbColor(0, 0, 0));
        for (int i=0; i<5; i++) {
                this->neo->SetPixelColor(i, RgbColor(r, g, b));
        }
        this->neo->Show();
};

/**
 * Set a rgb color (simple on-off)
 */
void WS281xStrip::rgb(int id, int r, int g, int b) {
        this->neo->SetPixelColor(id, RgbColor(r, g, b));
        this->neo->Show();
};
