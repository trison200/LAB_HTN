#include "global.h"

QueueHandle_t sensorQueue = nullptr;
QueueHandle_t configQueue = nullptr;
QueueHandle_t xQueueLed = nullptr;
QueueHandle_t xQueueNeo = nullptr;

SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
SemaphoreHandle_t semLedTemp = nullptr;
SemaphoreHandle_t semNeo = nullptr;

void initSharedQueues()
{
	if (sensorQueue == nullptr)
	{
		sensorQueue = xQueueCreate(1, sizeof(SensorData));
	}
	if (configQueue == nullptr)
	{
		configQueue = xQueueCreate(1, sizeof(CoreConfig));
	}
	if (xQueueLed == nullptr)
	{
		xQueueLed = xQueueCreate(1, sizeof(SensorData));
	}
	if (xQueueNeo == nullptr)
	{
		xQueueNeo = xQueueCreate(1, sizeof(SensorData));
	}
	if (semLedTemp == nullptr)
	{
		semLedTemp = xSemaphoreCreateBinary();
	}
	if (semNeo == nullptr)
	{
		semNeo = xSemaphoreCreateBinary();
	}
}

bool updateSensorData(const SensorData &data)
{
	if (sensorQueue == nullptr)
	{
		return false;
	}
	return xQueueOverwrite(sensorQueue, &data) == pdTRUE;
}

bool getLatestSensorData(SensorData &out)
{
	if (sensorQueue == nullptr)
	{
		return false;
	}
	return xQueuePeek(sensorQueue, &out, 0) == pdTRUE;
}

bool updateCoreConfig(const CoreConfig &config)
{
	if (configQueue == nullptr)
	{
		return false;
	}
	return xQueueOverwrite(configQueue, &config) == pdTRUE;
}

bool getLatestCoreConfig(CoreConfig &out)
{
	if (configQueue == nullptr)
	{
		return false;
	}
	return xQueuePeek(configQueue, &out, 0) == pdTRUE;
}