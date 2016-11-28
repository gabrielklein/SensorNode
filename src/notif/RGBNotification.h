//
// This class is used for "notifications" on a RGB PIN
//
#ifndef RGBNOTIFICATION_H
#define RGBNOTIFICATION_H


#include "Notification.h"
#include <Arduino.h>

class RGBNotification : public Notification {
public:
        RGBNotification(uint8_t r, uint8_t g, uint8_t b);
        ~RGBNotification();
        void ready();
        void off();
        void error();
        void processing();
        void loop();
        void setup();
private:
  void rgb(bool r, bool g, bool b);
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

#endif
