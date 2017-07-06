#include "Led.h"

Led::Led(FileServ *fileServ) {
        this->keyStore.setup("Led", fileServ);
        ledCountUpdated();
};

Led::~Led() {
        delete(this->neo);
};


/**
 * Switch on or off the internal led
 */
void Led::buildinLed(bool ison) {
        // Create strange issues with leds
        //    pinMode(BUILTIN_LED1, ison);
}

/**
 * Called when we have updated the configs.
 */
void Led::ledCountUpdated() {

        doCallLedCountUpdated = false;

        String k = this->keyStore.getValue("count");
        int l = k.toInt();
        if (l<=0 || l > 1000) {
                l = 10;
                this->keyStore.setValue("count", String(l));
        }

        if (this->countLed == l && this->neo!=NULL) {
                return;
        }

        this->countLed = l;
        if (this->neo != NULL) {
                this->neo->ClearTo(RgbColor(0, 0, 0));
                this->neo->Show();
                delete(this->neo);
        }

        this->neo = new NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart800KbpsMethod>(this->countLed, 2 /*port*/);

}

/**
 * Called at setup time. Use this call to initialize some data.
 */
void Led::setup() {

        // Create strange issues
        //pinMode(BUILTIN_LED1, OUTPUT);

        this->neo->Begin();
        //this->neo->ClearTo(RgbColor(0, 0, 0));
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
String Led::servName() {
        return "led";
}

/**
 * Register services on this webserver
 */
void Led::servRegister(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/led/set", HTTP_GET, [&] () {
                this->servSet();
        });
        webServer->on("/led/get", HTTP_GET, [&] () {
                this->servGet();
        });
        webServer->on("/led/config", HTTP_GET, [&] () {
                this->keyStore.servConfig(this->webServer);
                doCallLedCountUpdated = true;
        });
        //webServer->on("/ap/scan.do", HTTP_GET, [&] () {this->getScan(); });
}

/**
 * Get current led configuration
 */
void Led::servGet() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        root["count"] = this->countLed;
        JsonObject& l = root.createNestedObject("led");
        char buff[10];
        for (int i=0; i<this->countLed; i++) {
                RgbColor c = this->neo->GetPixelColor(i);
                HtmlColor h = HtmlColor(c);
                int l2 = h.ToNumericalString(buff, 10);
                String s(buff);
                String k(i);
                l["l"+k] = s;
        }

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

}


/**
 * Set a led exemple: /strip/set?l=0,1&r=125&g=55&b=220
 */
void Led::servSet() {

        String l = "";
        int r = 0;
        int g = 0;
        int b = 0;
        String rgb = "";

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
                if (key.equals("rgb")) {
                        rgb = value;
                }
        }

        RgbColor c = RgbColor(r, g, b);
        if (!rgb.startsWith("#")) {
                rgb = "#"+rgb;
        }
        if (rgb.length()==7) {
                HtmlColor htmlC;
                htmlC.Parse<HtmlColorNames>(rgb);
                Serial.println(rgb);
                c = RgbColor(htmlC);
        }

        if (l.equals("")) {
                ServerUtil::sendFail(this->webServer, 1, "l parameter is not defined");
        }
        else if (l.equals("*")) {
                for (int i=0; i<countLed; i++) {
                        this->neo->SetPixelColor(i, c);
                }
                doCallNeoShow = true;
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
                doCallNeoShow = true;
                ServerUtil::sendSuccess(this->webServer);
        }


}

/**
 * Do actions in the background
 */
void Led::loop() {


        if (doCallLedCountUpdated) {
                ledCountUpdated();
        }

        if (doCallNeoShow) {
                doCallNeoShow = false;
                this->neo->Show();
        }

        return;

        counter1++;

        if (counter1>2500) {

                RgbColor c = RgbColor(0,0,0);

                this->neo->SetPixelColor(counter2, c);
                counter1 = 0;
                counter2++;
                if (counter2>=countLed) {
                        counter2 = 0;

                        counter3++;
                        if (counter3>5) {
                                counter3 = 0;
                        }

                }

                switch (counter3) {
                case 0:
                        c = RgbColor(50,0,0);
                        break;
                case 1:
                        c = RgbColor(0,50,0);
                        break;
                case 2:
                        c = RgbColor(0,0,50);
                        break;
                case 3:
                        c = RgbColor(50,0,50);
                        break;
                case 4:
                        c = RgbColor(50,50,0);
                        break;
                case 5:
                        c = RgbColor(0,50,50);
                        break;
                }
                this->neo->SetPixelColor(counter2, c);
                this->neo->Show();
                counter1 = 0;
        }
}

/**
 * Set a rgb color (simple on-off)
 * Warning Show is called in "loop" to avoid issues with leds
 * isLoopSetupTask must be set to true if we are calling from the loop or setup function.
 */
void Led::rgb(int r, int g, int b, bool isLoopSetupTask) {
        this->neo->ClearTo(RgbColor(0, 0, 0));
        for (int i=0; i<5 && i<this->countLed; i++) {
                this->neo->SetPixelColor(i, RgbColor(r, g, b));
        }
        if (isLoopSetupTask) {
                this->neo->Show();
        }
        else {
                doCallNeoShow = true;
        }
};

/**
 * Set a rgb color (simple on-off)
 * Warning Show is called in "loop" to avoid issues with leds
 * isLoopSetupTask must be set to true if we are calling from the loop or setup function.
 */
void Led::rgb(int id, int r, int g, int b) {
        this->neo->SetPixelColor(id, RgbColor(r, g, b));
        this->neo->Show();
};

/**
 * Set a rgb color (simple on-off)
 * Warning Show is called in "loop" to avoid issues with leds
 */
//void Led::rgb(int id, int r, int g, int b) {
//        this->neo->SetPixelColor(id, RgbColor(r, g, b));
//        doCallNeoShow = true;
//        //this->neo->Show();
//};
