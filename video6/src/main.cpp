#include <Arduino.h>

void TaskLEDControl(void *pvParameters) {
  pinMode(GPIO_NUM_48, OUTPUT); // Initialize LED pin
  // int ledState = 0;
  while(1) {
    digitalWrite(GPIO_NUM_48, HIGH);
    // if (ledState == 0) {
    //   digitalWrite(GPIO_NUM_48, HIGH); // Turn ON LED
    // } else {
    //   digitalWrite(GPIO_NUM_48, LOW); // Turn OFF LED
    // }
    // ledState = 1 - ledState;
    Serial.println("LED ON");
    vTaskDelay(2000);
  }
}


void setup() {
  Serial.begin(115200);
  xTaskCreate(TaskLEDControl, "LED Control", 2048, NULL, 2, NULL);
}

void loop() {
}