#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

struct SensorData {
	float temperature = 0.0f;
	float humidity = 0.0f;
	int tempState = 0;
	int humiState = 0;
};

struct CoreConfig {
	String ssid;
	String pass;
	String token;
	String server;
	String port;
};

extern QueueHandle_t sensorQueue;
extern QueueHandle_t configQueue;
extern QueueHandle_t xQueueLed;
extern QueueHandle_t xQueueNeo;
extern SemaphoreHandle_t xBinarySemaphoreInternet;
extern SemaphoreHandle_t semLedTemp;
extern SemaphoreHandle_t semNeo;

void initSharedQueues();
bool updateSensorData(const SensorData &data);
bool getLatestSensorData(SensorData &out);
bool updateCoreConfig(const CoreConfig &config);
bool getLatestCoreConfig(CoreConfig &out);
#endif