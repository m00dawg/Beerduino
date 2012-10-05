Yet Another Beerduino
=====================

An Arduino-Based solution to control a freezer for brewing / fermenting

Currently, most development is being done in Aquariduino
(see: https://github.com/m00dawg/Aquariduino). The two projects are fairly
similar and it is easier to test functionality with Aquaduino. As such,
new work will happen there for a while and will be back-ported to this project.

Hardware Requirements:

- Arduino Uno (most compatibles should work)
- Relay/AC Switch (PowerSwitch Tail II)
- Waterproof Temperature Sensor (DS18B20)
- Adafruit's RGBLCDShield (or something that behaves like it)

Libraries:

- OneWire (http://www.pjrc.com/teensy/td_libs_OneWire.html)
- DallasTemperature (http://milesburton.com/Dallas_Temperature_Control_Library)
- Adafruit RGB LCD Shield (https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library)

Pins Used (Defaults):

- Digital Pin 12: Relay to Control Aquarium Heater
- Digital Pin 7: Interface to 1-Wire Temperature Sensor
- Analog Pins 4,5: LCD Shield (I2C Bus)

Assembly:

For a proper build-guide, go here:

http://www.moocowproductions.org/articles/beerduino/

