# SensorNode

SensorNode is an opensource framework to configure access to different modules on the ESP8266 NodeMCU.

# What you can currently do?

* Access temperature, time, leds using a REST API / JSON.
* You can configure wireless access using a web interface (captive portal).
* You can get time and configure timezones once you have configured internet access.
* Control LEDs using the web interface (or calling an api).
* Switch on-off a relay (using an api call).

# What I expect to do?

* Do actions when we are out of some temperature (send an alert).
* Improve the ugly UI/UX, make it mobile friendly.
* Fix some issues with RGB Leds.
* Create a sunrise, sunset effect.
* Use a web interface to control relays.
* Draw some schemas and share some pictures / videos.
* Switch on and off relay using time.
* Switch on and off relay using temperature.

Help more than welcome!

# Configuration
Settings.h contains the configuration for the pins

# Needed libraries

pio lib list

* 64:   ArduinoJson (5.8.4)
* 54:   DallasTemperature (3.7.7)
* 1:    OneWire (2.3.2)
* 76:   Timezone (8e96486b2c)
* 44:   Time (1.5)
* 551:  NTPClient (3.1.0)
* 547:  NeoPixelBus (2.2.7)
* 19:   DHT sensor library (1.3.0)
* 31:   Adafruit Unified Sensor (1.0.2)


You can install them by typing in your terminal (the icon with a > on the left of PlatformIO).

platformio lib install {library id}

Example for the pixel library: platformio lib install 679


# What is the current status

* You can configure a captive portal
* You can control the leds (no ui yet)
* You can get temperature
* You can control 2 relays


# What you need?

- PlatformIO
- An ESP8266
- Some modules

# Supported modules

You may be interested to buy:
Please note that other modules are supported for each of these sensors type. It's the one I've tested.

* NodeMCU dev board

https://www.aliexpress.com/item/1PCS-Wireless-module-CH340-NodeMcu-V3-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266/32665100123.html

* WS2813 or WS2812 (RGB leds)

https://www.aliexpress.com/item/LED-WS2813-1m-4m-5m-New-product-WS2813-Smart-led-pixel-strip-IP30-IP67-DC5V-Zwart/32685607810.html

or

https://www.aliexpress.com/item/10-1000pcs-4-Pin-WS2812B-WS2812-LED-Chip-Heatsink-5V-5050-RGB-WS2811-IC-Built-in/32634454437.html

A few leds could be connected directly to signal port D4 and powered with 3.3v. I tried with 300.

The red, geen, blue level of each of them can be controlled individually.

I recommend to use a SN74AHCT125N if you want to use more than 5 at full power!

http://circuits.datasheetdir.com/102/SN74AHCT125-pinout.jpg

1: 1OE: Connect to GND

2: 1A: Input with port D4

3: 1Y: Output to "signal" pin of the leds.

7: GND to GND

14: Vcc to +5V

Connect the leds to the 5v power supply!

2x 1 Channel, 5V relay module KY-019

https://www.aliexpress.com/store/product/10PCS-LOT-1-Channel-5V-Relay-Module-for-SCM-Household-Appliance-Control/110055_752210006.html

Should work fine with 5v. You can connect them to port D7 and D8. Please note that they consume a lot of energy!

* DS18B20 Waterproof temperature sensors (Should be supported DS18B20, DS18S20, DS1822, DS1820)

https://www.aliexpress.com/item/DS18B20-Stainless-steel-package-1-meters-waterproof-DS18b20-temperature-probe-temperature-sensor-18B20-in-stock-high/670872130.html

You can connect multiple temperature modules together (I've tested 3 with success).

Link the red and blue wires together with a resistance of 4.7 kOhm. (A resistance between D1 and +3.3v)

You can connect the signal wire on D1.

* DHT22 temperature and humidity sensor.

Put a resistance of 4700 Ohm between +3.3v and "signal"

https://www.aliexpress.com/item/Free-shipping-DHT22-AM2302-replace-SHT11-SHT15-Humidity-temperature-and-humidity-sensor/1872664976.html

# How to deploy

All actions are done on the left panel.

To build: PlatformIO: Build

If you have any errors, verify that your libraries are well installed!

To deploy files (images, html, javascript),

click on : Run other target. PIO Upload SPIFFS image

This action will clear configurations!

Now click on PlatformIO: Upload

The leds will blink and you will see a new wireless access.

Connect to this wireless access and type: http://192.168.0.1/ in your browser.
