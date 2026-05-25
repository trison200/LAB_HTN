#ifndef __TEMP_HUMI_MONITOR__
#define __TEMP_HUMI_MONITOR__
#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include <DHT.h>
#include "global.h"

#define DHT_PIN 35
#define DHT_TYPE DHT11

void temp_humi_monitor(void *pvParameters);


#endif