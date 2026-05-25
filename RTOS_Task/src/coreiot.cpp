#include "coreiot.h"

// ----------- CONFIGURE THESE! -----------
const char* coreIOT_Server = "10.235.76.226";  
const char* coreIOT_Token = "g7drm1amhd3dchr379xu";   // Device Access Token
const int   mqttPort = 1883;
// ----------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);
constexpr int GPIO_CONTROL_PIN = 8;


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    CoreConfig config;
    if (!getLatestCoreConfig(config) || config.token.isEmpty()) {
      Serial.println("CORE_IOT_TOKEN is empty; check saved config.");
      delay(5000);
      continue;
    }
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect (username=token, password=empty)
    //if (client.connect("ESP32Client", coreIOT_Token, NULL)) {
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), config.token.c_str(), NULL)) {
        
      Serial.println("connected to CoreIOT Server!");
      client.subscribe("v1/devices/me/rpc/request/+");
      Serial.println("Subscribed to v1/devices/me/rpc/request/+");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  // Allocate a temporary buffer for the message
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';
  Serial.print("Payload: ");
  Serial.println(message);

  // Parse JSON
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* method = doc["method"];
  if (!method) {
    Serial.println("Invalid RPC: missing method");
    return;
  }

  if (strcmp(method, "setStateLED") == 0) {
    bool newState = false;
    if (doc["params"].is<bool>()) {
      newState = doc["params"].as<bool>();
    } else if (doc["params"].is<int>()) {
      newState = doc["params"].as<int>() != 0;
    } else if (doc["params"].is<const char*>()) {
      const char* params = doc["params"];
      if (params) {
        newState = (strcmp(params, "ON") == 0) || (strcmp(params, "on") == 0) ||
                   (strcmp(params, "1") == 0) || (strcmp(params, "true") == 0) ||
                   (strcmp(params, "TRUE") == 0);
      }
    }

    digitalWrite(GPIO_CONTROL_PIN, newState ? HIGH : LOW);
    Serial.printf("GPIO %d -> %s\n", GPIO_CONTROL_PIN, newState ? "ON" : "OFF");
  } else {
    Serial.print("Unknown method: ");
    Serial.println(method);
  }
}


void setup_coreiot(){

  //Serial.print("Connecting to WiFi...");
  //WiFi.begin(wifi_ssid, wifi_password);
  //while (WiFi.status() != WL_CONNECTED) {
  
  // while (isWifiConnected == false) {
  //   delay(500);
  //   Serial.print(".");
  // }

  while(1){
    if (xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY)) {
      break;
    }
    delay(500);
    Serial.print(".");
  }

  pinMode(GPIO_CONTROL_PIN, OUTPUT);
  digitalWrite(GPIO_CONTROL_PIN, LOW);


  Serial.println(" Connected!");

  CoreConfig config;
  if (getLatestCoreConfig(config))
  {
    client.setServer(config.server.c_str(), config.port.toInt());
  }
  client.setCallback(callback);

}

void coreiot_task(void *pvParameters){

    setup_coreiot();

    while(1){

        if (!client.connected()) {
            reconnect();
        }
        client.loop();

        // Sample payload, publish to 'v1/devices/me/telemetry'
        SensorData sensorData;
        if (getLatestSensorData(sensorData))
        {
          String payload = "{\"temperature\":" + String(sensorData.temperature) +  ",\"humidity\":" + String(sensorData.humidity) + "}";
          client.publish("v1/devices/me/telemetry", payload.c_str());
          Serial.println("Published payload: " + payload);
        }
        vTaskDelay(10000);  // Publish every 10 seconds
    }
}