#include "led_blinky.h"
#include "global.h"

void led_blinky(void *pvParameters){
  pinMode(LED_PIN, OUTPUT);
  SensorData data;
  int blinkDelay = 1000;
  
  while(1) {                        
    if (xSemaphoreTake(semLedTemp, 0) == pdTRUE) {
        if (xQueuePeek(xQueueLed, &data, 0) == pdTRUE) {
            if (data.tempState == 2) {
                blinkDelay = 3000; // CRITICAL: fast blink
            } else if (data.tempState == 1) {
                blinkDelay = 4000; // WARNING: medium blink
            } else {
                blinkDelay = 5000; // NORMAL: slow blink
            }
        }
    }

    digitalWrite(LED_PIN, HIGH);  // turn the LED ON
    vTaskDelay(pdMS_TO_TICKS(blinkDelay));
    digitalWrite(LED_PIN, LOW);  // turn the LED OFF
    vTaskDelay(pdMS_TO_TICKS(blinkDelay));
  }
}