#include "temp_humi_monitor.h"
#include "task_webserver.h"
#include "Wire.h"

DHT dht11(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27,16,2);


void temp_humi_monitor(void *pvParameters){

    Wire.begin(14,21);
    Serial.begin(115200);
    dht11.begin();
    
    lcd.begin();
    lcd.backlight();

    while (1){
        
        float temperature = dht11.readTemperature();
        float humidity = dht11.readHumidity();

        

        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
        }

        SensorData sensorData;
        sensorData.temperature = temperature;
        sensorData.humidity = humidity;

        if (temperature < 28) {
            sensorData.tempState = 0;
        } 
        else if (temperature < 32) {
            sensorData.tempState = 1;
        } 
        else {
            sensorData.tempState = 2;
        }

        // Humidity state
        if (humidity < 60) {
            sensorData.humiState = 0;
        } 
        else if (humidity < 85) {
            sensorData.humiState = 1;
        } 
        else {
            sensorData.humiState = 2;
        }

        updateSensorData(sensorData);
        
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(temperature, 1);
        lcd.print("C H:");
        lcd.print(humidity, 0);
        lcd.print("%");

        lcd.setCursor(0, 1);
        if (sensorData.tempState == 2 && sensorData.humiState == 2) {
            lcd.print("CRITICAL");
        }
        else if (sensorData.tempState == 1 && sensorData.humiState == 1) {
            lcd.print("WARNING");
        }
        else {
            lcd.print("NORMAL");
        }

        // Send latest data to queues
        xQueueOverwrite(xQueueLed, &sensorData);
        xQueueOverwrite(xQueueNeo, &sensorData);

        // Notify LED and NeoPixel tasks
        if(semLedTemp != nullptr) xSemaphoreGive(semLedTemp);
        if(semNeo != nullptr) xSemaphoreGive(semNeo);
        
        String json = "{\"page\":\"sensor\",\"value\":{\"temp\":" + String(temperature) + ",\"humi\":" + String(humidity) + "}}";
        Webserver_sendata(json);
        
        vTaskDelay(5000);
    }
    
}