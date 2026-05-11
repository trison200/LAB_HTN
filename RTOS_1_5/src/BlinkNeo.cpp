#include "BlinkNeo.h"

void blinkNeo(void *pvParameters)
{
    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();
    while (1)
    {
        strip.setPixelColor(0, strip.Color(0, 0, 255));
        strip.show();

        vTaskDelay(1000);

        strip.clear();
        strip.show();

        vTaskDelay(1000);
    }
}

// void blinkNeo(void *pvParameters)
// {
//     Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
//     strip.begin();
//     strip.clear();
//     strip.show();
//     while (1)
//     {
//         if (glob_led2_state)
//         {
//             strip.setPixelColor(0, strip.Color(0, 0, 255));
//             strip.show();
//         }
//         else
//         {
//             strip.clear();
//             strip.show();
//         }
//         vTaskDelay(1000);
//     }
// }