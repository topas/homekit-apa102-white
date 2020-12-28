# Homekit APA102 (white only)

Recently I found Arduino library [Homekit-ESP8266](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266) and usage is pretty streightforward. You can you use your favorite Ardiono tooling and libraries. It's great because you can level up your Arduino projects with Homekit easily. Just put it on ESP and adjust wiring a bit.

This projects is modified [RGB WS2812 Example](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266/tree/master/examples/Example05_WS2812_Neopixel) for my warm white APA102 LED strip in my living room.

## How to use it

- Set your SSID and password in `wifi_setup.h`
- Build & upload it via [Platformio](https://platformio.org)

## Parts (Aliexpress links)

- [White APA102 LED Strip](https://www.aliexpress.com/item/32451954771.html?spm=a2g0s.9042311.0.0.27424c4d5lSh2B)
- [ESP8266 NodeMCU](https://www.aliexpress.com/item/32813713134.html?spm=a2g0s.9042311.0.0.40de4c4dSAfRbb)
- 74HCT125
- Strong power supply

## Wiring

Because of ESP8266 boards use 3.3V logic and APA102 LED stripes has 5V you need level shifter. I used 74HCT125 for converting data and clock signal. Pins can be configured in `main.ino`
