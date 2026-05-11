#include "global.h"
#include "TempHumid.h"
#include "BlinkLed.h"
#include "BlinkNeo.h"
#include "mainserver.h"
#include "TinyML.h"

void setup()
{
  Serial.begin(115200);

  xTaskCreate(getTempHumid, "Task temp humi monitor", 2048, NULL, 2, NULL);
  xTaskCreate(blinkLed, "Task blinky", 2048, NULL, 2, NULL);
  xTaskCreate(blinkNeo, "Task blinky neo pixels", 2048, NULL, 2, NULL);
  xTaskCreate(main_server_task, "Task Main Server", 8192, NULL, 2, NULL);
  // xTaskCreate(tiny_ml_task, "Tiny ML Task", 2048, NULL, 2, NULL);
}

void loop()
{
}
