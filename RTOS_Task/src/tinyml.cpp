#include "tinyml.h"

// Globals, for the convenience of one-shot setup.
namespace
{
    tflite::ErrorReporter *error_reporter = nullptr;
    const tflite::Model *model = nullptr;
    tflite::MicroInterpreter *interpreter = nullptr;
    TfLiteTensor *input = nullptr;
    TfLiteTensor *output = nullptr;
    constexpr int kTensorArenaSize = 8 * 1024; // Adjust size based on your model
    uint8_t tensor_arena[kTensorArenaSize];
} // namespace

void setupTinyML()
{
    Serial.println("TensorFlow Lite Init....");
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    model = tflite::GetModel(tempHumiModel); // g_model_data is from model_data.h
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        error_reporter->Report("Model provided is schema version %d, not equal to supported version %d.",
                               model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    static tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk)
    {
        error_reporter->Report("AllocateTensors() failed");
        return;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);

    Serial.println("TensorFlow Lite Micro initialized on ESP32.");
}

void tiny_ml_task(void *pvParameters)
{

    setupTinyML();
    
    const int BUZZER_PIN = 39;
    pinMode(BUZZER_PIN, OUTPUT);

    while (1)
    {

        // Prepare input data (e.g., sensor readings)
        // For a simple example, let's assume a single float input
        SensorData sensorData;
        if (!getLatestSensorData(sensorData))
        {
            vTaskDelay(1000);
            continue;
        }
        input->data.f[0] = sensorData.temperature;
        input->data.f[1] = sensorData.humidity;

        // Run inference
        TfLiteStatus invoke_status = interpreter->Invoke();
        if (invoke_status != kTfLiteOk)
        {
            error_reporter->Report("Invoke failed");
            return;
        }

        // Get and process output
        float result = output->data.f[0];
        Serial.print("Inference result: ");
        Serial.println(result);

        static int consecutive_alarms = 0; // Biến lưu số lần bất thường liên tiếp
        const float THRESHOLD = 0.80;    
        
        if (result >= THRESHOLD) 
        {
            consecutive_alarms++;
            Serial.print("Phát hiện bất thường lần ");
            Serial.println(consecutive_alarms);

            if (consecutive_alarms >= 3) 
            {
                Serial.println("Môi trường có sự cố!");
                digitalWrite(BUZZER_PIN, HIGH);
            }
        } 
        else 
        {
            consecutive_alarms = 0;
            digitalWrite(BUZZER_PIN, LOW);
        }

        vTaskDelay(5000);
    }
}