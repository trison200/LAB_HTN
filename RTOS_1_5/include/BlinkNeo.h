#ifndef __BLINK_NEO__
#define __BLINK_NEO__
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "global.h"

#define NEO_PIN 45
#define LED_COUNT 1

void blinkNeo(void *pvParameters);

#endif