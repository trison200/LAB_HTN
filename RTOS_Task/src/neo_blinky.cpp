#include "neo_blinky.h"
#include "global.h"

void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();
    
    SensorData data;
    int r = 0, g = 255, b = 0; // Default green for NORMAL

    while(1) {                          
        if (xSemaphoreTake(semNeo, 0) == pdTRUE) {
            if (xQueuePeek(xQueueNeo, &data, 0) == pdTRUE) {
                if (data.humiState == 2) {
                    r = 255; g = 0; b = 0; // CRITICAL: Red
                } else if (data.humiState == 1) {
                    r = 255; g = 165; b = 0; // WARNING: Orange
                } else {
                    r = 0; g = 255; b = 0; // NORMAL: Green
                }
            }
        }

        strip.setPixelColor(0, strip.Color(r, g, b));
        strip.show(); 
        vTaskDelay(pdMS_TO_TICKS(500));

        strip.setPixelColor(0, strip.Color(0, 0, 0));
        strip.show();

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}