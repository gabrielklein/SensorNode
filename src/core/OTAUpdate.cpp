#include "OTAUpdate.h"

/**
 * Default keystore
 */
OTAUpdate::OTAUpdate() {
}


OTAUpdate::~OTAUpdate() {

}

void OTAUpdate::loop() {
        ArduinoOTA.handle();
}

void OTAUpdate::setup() {

        Serial.println("OTAUpdate: Enable service");

        // Port defaults to 8266
        #ifdef OTA_PORT
        ArduinoOTA.setPort(OTA_PORT);
        #endif

        // Hostname defaults to esp8266-[ChipID]
        // ArduinoOTA.setHostname("myesp8266");

        // No authentication by default
        #ifdef OTA_PASSWORD
        ArduinoOTA.setPassword(OTA_PASSWORD);
        #endif

        // Password can be set with it's md5 value as well
        // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
        // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

        ArduinoOTA.onStart([]() {
                String type;
                // Not supported (Yet)àà
                //if (ArduinoOTA.getCommand() == U_FLASH)
                //        type = "sketch";
                //else // U_SPIFFS
                //        type = "filesystem";

                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                // A bit crude...
                SPIFFS.end();
        });
        ArduinoOTA.onEnd([]() {
                Serial.println("OTAUpdate: Finished");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
                Serial.printf("OTAUpdate: Progress %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
                Serial.printf("OTAUpdate: Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) Serial.println("OTAUpdate: Auth Failed");
                else if (error == OTA_BEGIN_ERROR) Serial.println("OTAUpdate: Begin Failed");
                else if (error == OTA_CONNECT_ERROR) Serial.println("OTAUpdate: Connect Failed");
                else if (error == OTA_RECEIVE_ERROR) Serial.println("OTAUpdate: Receive Failed");
                else if (error == OTA_END_ERROR) Serial.println("OTAUpdate: End Failed");
        });
        ArduinoOTA.begin();
}
