#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <string.h>
#include "FS.h"
#include "notif/Notification.h"

class Config {


public:
        // Construct using the notification object
        Config(Notification* notification);
        ~Config();
        // Save configuration
        bool saveConfig();
        // Get a value in the configuration
        String getValue(String key);
        // Set a value in the configuration
        void setValue(String key, String value);
        // Reset the configuration with the default values
        void reset();
        // We store only this amount of "key/values"
        static const int MAX_ITEM = 64;
        // Get all values in a k1=v1\nk2=v3\n...
        String getValues();

private:
        Notification* notif;
        String key[MAX_ITEM];
        String value[MAX_ITEM];
};

#endif
