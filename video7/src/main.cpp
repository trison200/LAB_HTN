#include <Arduino.h>
#include "DHT20.h"

DHT20 dht20;

void temp_humi_monitor(void *pvParameters)
{

  Wire.begin(11, 12);
  Serial.begin(115200);
  dht20.begin();

  while (1)
  {
    dht20.read();
    float temperature = dht20.getTemperature();
    float humidity = dht20.getHumidity();

    if (isnan(temperature) || isnan(humidity))
    {
      Serial.println("Failed to read from DHT sensor!");
      temperature = humidity = -1;
      return;
    }

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");

    vTaskDelay(5000);
  }
}

void TaskLEDControl(void *pvParameters)
{
  Serial.begin(115200);
  pinMode(GPIO_NUM_48, OUTPUT);
  int ledState = 0;
  while (1)
  {
    if (ledState == 0)
    {
      digitalWrite(GPIO_NUM_48, HIGH); // Turn ON LED
    }
    else
    {
      digitalWrite(GPIO_NUM_48, LOW); // Turn OFF LED
    }
    ledState = 1 - ledState;
    Serial.println("LED toggle");
    vTaskDelay(2000);
  }
}

void setup()
{
  xTaskCreate(temp_humi_monitor, "temp_humi", 2048, NULL, 2, NULL);
  // xTaskCreate(TaskLEDControl, "led control", 2048, NULL, 2, NULL);
}

void loop()
{
}