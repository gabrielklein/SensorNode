//
// This class is used for "notifications" on a RGB PIN
//
#ifndef NONOTIFICATION_H
#define NONOTIFICATION_H

#include "Notification.h"

class NoNotification : public Notification {
public:
        NoNotification();
        ~NoNotification();
        void ready();
        void error();
        void processing();
        void loop();
        void setup();
        void off();
};

#endif
