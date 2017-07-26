#ifndef IMQTT_H
#define IMQTT_H

#include <Arduino.h>

class IMQTT {

public:

// Shortname of this service
// Same as IServer.h for consistance
virtual String servName();

// When we receive a message
// Example: A3E1 switch 0 0
// It means switch off first switch on sensor A3E1
// You will receive only as a string: 0 0
virtual void servSetMQTT(String mess);

// Return "" if no message to be sent.
// Return the message to be send if we want to send a message to MQTT.
// SensorNode name and "servName will be added"
// Example: A3E1 temp 31
virtual String servGetMQTT();

};

#endif
