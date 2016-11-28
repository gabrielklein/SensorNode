//
// This class is used for "notifications" on a RGB PIN
//
#ifndef NOTIFICATION_H
#define NOTIFICATION_H


class Notification {
public:
        // When we are ready
        virtual void ready();
        // When we have an error
        virtual void error();
        // When we are "processing"
        virtual void processing();
        // Call in the main loop
        virtual void loop();
        // Call in the "setup"
        virtual void setup();
        // Remove all "notifications" (switch off leds)
        virtual void off();
};

#endif
