#include "Config.h"

Config::Config(Notification* notification) {

        this->notif = notification;

        File f = SPIFFS.open("/config/config.json", "r");
        if (f) {

                for(int i=0; i<Config::MAX_ITEM; i++) {

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
                }
                f.close();
                return;
        }
        else {
                this->reset();
        }



}

String Config::getValues() {
        String s;
        for(int i=0; i<Config::MAX_ITEM; i++) {
                s+=key[i];
                s+="=";
                s+=value[i];
                s+="\n";
        }
        return s;
}

Config::~Config() {
}

/**
 * Get json objetct
 */
void Config::reset() {

        for(int i=0; i<Config::MAX_ITEM; i++) {
                key[i] = "";
                value[i] = "";
        }

        int i=0;
        key[i] = "wireless_password";
        value[i] = "";
        i++;
        key[i] = "wireless_ssid";
        value[i] = "";
        i++;
        key[i] = "ip_dhcp";
        value[i] = "1";
        i++;
        key[i] = "ip_ip";
        value[i] = "192.168.0.100";
        i++;
        key[i] = "ip_subnet";
        value[i] = "255.255.255.0";
        i++;
        key[i] = "ip_gateway";
        value[i] = "192.168.0.1";
        i++;
        key[i] = "ip_dns";
        value[i] = "8.8.8.8";
        i++;
        key[i] = "remote_use";
        value[i] = "0";
        i++;
        key[i] = "remote_url";
        value[i] = "http://yousite.com/sensornode.php";
        i++;

        this->saveConfig();
}

bool Config::saveConfig() {

        Serial.println("SaveConfig");
        Serial.println((long)this);

        File f = SPIFFS.open("/config/config.json", "w");
        if (!f) {
                Serial.println("Failed to open config file for writing");
                return false;
        }

        for(int i=0; i<Config::MAX_ITEM; i++) {
                f.println(key[i]);
                f.println(value[i]);
        }
        f.close();

        return true;
}

String Config::getValue(String k) {

        for(int i=0; i<Config::MAX_ITEM; i++) {
                if (key[i].equals(k)) {
                        return value[i];
                }
        }
        return String("");
}

void Config::setValue(String k, String v) {
        // Find key
        for(int i=0; i<Config::MAX_ITEM; i++) {
                if (key[i].equals(k)) {
                        value[i] = v;
                        return;
                }
        }
        // Not found key
        for(int i=0; i<Config::MAX_ITEM; i++) {
                if (key[i].equals("")) {
                        key[i] = k;
                        value[i] = v;
                        return;
                }
        }

        // No place anymore...
        notif->error();
}
