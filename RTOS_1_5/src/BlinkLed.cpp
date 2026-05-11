#include "BlinkLed.h"

// void blinkLed(void *pvParameters)
// {
//     pinMode(LED_GPIO, OUTPUT);

//     while (1)
//     {
//         digitalWrite(LED_GPIO, HIGH);
//         vTaskDelay(1000);
//         digitalWrite(LED_GPIO, LOW);
//         vTaskDelay(1000);
//     }
// }

void blinkLed(void *pvParameters)
{
    pinMode(LED_GPIO, OUTPUT);

    while (1)
    {
        if (glob_led1_state)
        {
            digitalWrite(LED_GPIO, HIGH);
        }
        else
        {
            digitalWrite(LED_GPIO, LOW);
        }
        vTaskDelay(1000);
    }
}