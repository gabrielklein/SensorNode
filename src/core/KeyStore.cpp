#include "KeyStore.h"

/**
 * Default keystore
 */
KeyStore::KeyStore() {
}

/**
 * Serv configuration
 */
void KeyStore::servConfig(ESP8266WebServer *webServer) {

        int currentArgCount = webServer->args();

        if (currentArgCount>0) {
                for (int i = 0; i < currentArgCount; ++i) {
                        String k = webServer->argName(i);
                        String v = webServer->arg(i);
                        if (!this->setValue(k, v)) {
                                ServerUtil::sendFail(webServer, 3, "We cannot store an extra parameters, config file is full.");
                        }
                }
                this->saveKeyStore();
        }

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        root["path"] = this->filePath;
        root["maxItems"] = CONFIG_MAX_ITEM;
        JsonObject& t = root.createNestedObject("config");

        for(int i=0; i<CONFIG_MAX_ITEM; i++) {
                //JsonObject& x = tempJ.createNestedObject();
                if (key[i] != NULL) {
                        t[key[i]] = value[i];
                }
        }

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);
}

/**
 * Create a keyvalue file with a specific name
 */
bool KeyStore::setup(String name, FileServ* fileServ) {

        this->filePath = "/config/"+name+".kst";

        File f = fileServ->openR(this->filePath);
        if (f) {
                Serial.print("KeyStore: Opening file with success: ");
                Serial.println(this->filePath);
                for(int i=0; i<CONFIG_MAX_ITEM; i++) {

                        if (f.available()>0) {
                                key[i] = f.readStringUntil('\n');
                                key[i].trim();
                        }
                        else {
                                key[i] = "";
                        }
                        if (f.available()>0) {
                                value[i] = f.readStringUntil('\n');
                                value[i].trim();
                        }
                        else {
                                value[i] = "";
                        }
                        if (key[i].length()>0)
                                Serial.println(key[i]+" => "+value[i]);
                }
                f.close();
                return true;
        }
        else {
                Serial.print("KeyStore: File not found: ");
                Serial.println(this->filePath);
                return false;
        }

}

KeyStore::~KeyStore() {
}

/**
 * Save keystore
 */
bool KeyStore::saveKeyStore() {

        String name = "SaveConfig";
        name = name+this->filePath;
        Serial.println("SaveConfig");
        Serial.println((long)this);

        File f = fileServ->openW(this->filePath);
        if (!f) {
                Serial.println("KeyStore: Failed to open config file for writing: "+this->filePath);
                return false;
        }

        Serial.println("KeyStore: Writing file with success: "+this->filePath);
        for(int i=0; i<CONFIG_MAX_ITEM; i++) {
                if (key[i] != NULL) {
                        Serial.println(key[i]+" => "+value[i]);
                        f.println(key[i]);
                        f.println(value[i]);
                }
        }

        f.close();

        return true;
}

String KeyStore::getValue(String k) {

        for(int i=0; i<CONFIG_MAX_ITEM; i++) {
                if (key[i].equals(k)) {
                        return value[i];
                }
        }
        return String("");
}

int KeyStore::getValueInt(String key) {
        String v = getValue(key);
        return v.toInt();
}

float KeyStore::getValueFloat(String key) {
        String v = getValue(key);
        return v.toFloat();
}

bool KeyStore::getValueBool(String key) {
        String v = getValue(key);
        return v.toInt() != 0;
}

/**
 * Set a key - value pair
 */
bool KeyStore::setValue(String k, String v) {
        // Find key
        for(int i=0; i<CONFIG_MAX_ITEM; i++) {
                if (key[i].equals(k)) {
                        if (v == NULL || v.length() == 0) {
                                // delete the key
                                key[i] = "";
                        }
                        else {
                                value[i] = v;
                        }
                        return true;
                }
        }

        // Don't change if value is not set.
        if (v == NULL || v.length() == 0)
                return true;

        // Key not found
        for(int i=0; i<CONFIG_MAX_ITEM; i++) {
                if (key[i].equals("")) {
                        key[i] = k;
                        value[i] = v;
                        return true;
                }
        }

        Serial.println("Cannot add new key, empty slot.");
        return false;
}
