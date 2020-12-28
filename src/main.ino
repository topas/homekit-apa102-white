#include <Arduino.h>
#include <arduino_homekit_server.h>
#include <homekit/types.h>
#include "wifi_setup.h"

#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>

#define NUM_LEDS 310
#define DATA_PIN D2
#define CLOCK_PIN D1

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

bool is_on = false;
float current_brightness = 0;
CRGB leds[NUM_LEDS];


void setup() {
    Serial.begin(115200);
    wifi_connect(); // in wifi_setup.h

    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
    delay(1000);
    my_homekit_setup();
}

void loop() {
    my_homekit_loop();
    delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in accessory.c

extern "C" homekit_server_config_t accessory_config;
extern "C" homekit_characteristic_t cha_on;
extern "C" homekit_characteristic_t cha_bright;

static uint32_t next_heap_millis = 0;

void my_homekit_setup() {

    cha_on.setter = set_on;
    cha_on.getter = get_on;
    cha_bright.setter = set_bright;
    cha_bright.getter = get_bright;

    arduino_homekit_setup(&accessory_config);
}

void my_homekit_loop() {
    arduino_homekit_loop();
    const uint32_t t = millis();
    if (t > next_heap_millis) {
        // show heap info every 5 seconds
        next_heap_millis = t + 5 * 1000;
        LOG_D("Free heap: %d, HomeKit clients: %d",
            ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

    }
}

homekit_value_t get_on() {
    homekit_value_t v;
    v.format = homekit_format_bool;
    v.bool_value = is_on;
    return v;
}

homekit_value_t get_bright() {
    homekit_value_t v;
    v.format = homekit_format_int;
    v.int_value = current_brightness;
    return v;
}

void set_on(const homekit_value_t v) {
    bool on = v.bool_value;
    cha_on.value.bool_value = on; //sync the value

    if (on) {
        is_on = true;
        Serial.println("On");
    }
    else {
        is_on = false;
        Serial.println("Off");
    }

    updateLedStrip();
}

void set_bright(const homekit_value_t v) {
    Serial.println("set_bright");
    int bright = v.int_value;
    cha_bright.value.int_value = bright; // sync the value

    current_brightness = bright;

    updateLedStrip();
}


void updateLedStrip()
{
    if (!is_on) {
        setLedBrightness(0);
    }
    else {
        setLedBrightness(current_brightness);
    }
}

void setLedBrightness(int ledBrightness)
{
    const uint8_t toRgb = (ledBrightness / 100.0) * 255;
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB(toRgb, toRgb, toRgb);
    }
    FastLED.show();
}
