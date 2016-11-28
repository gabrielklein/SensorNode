# SensorNode
SensorNode is an opensource framework to configure access to different modules on the ESP8266 NodeMCU.

*** WORK IN PROGRESS! ***

It's really a first draft! Quick and dirty. Don't expect anything amazing out of it.
I share it now as I think it mays be a source of inspiration for other people.

# What is needed

Settings.h contains the configuration for the pins

* You need a "PULLUP" Switch on D1.
If you start and press this switch, you will start in access point (captive) mode.
The first time, you will be started in captive mode.

* You need a RGB led on the D2, D3, D4 pins

Red: D2
Green: D3
Blue: D4


# What is the current status
You can configure part of a "captive portal".
Then it will connect to a wireless network.

# What you need?
- PlatformIO
- An ESP8266

Read "setup.txt" to know how to deploy it to your ESP8266
