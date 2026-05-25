#include "task_check_info.h"

void Load_info_File()
{
  File file = LittleFS.open("/info.dat", "r");
  if (!file)
  {
    return;
  }
  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
  }
  else
  {
    CoreConfig config;
    config.ssid = String(doc["WIFI_SSID"] | "");
    config.pass = String(doc["WIFI_PASS"] | "");
    config.token = String(doc["CORE_IOT_TOKEN"] | "");
    config.server = String(doc["CORE_IOT_SERVER"] | "");
    config.port = String(doc["CORE_IOT_PORT"] | "");
    updateCoreConfig(config);
  }
  file.close();
}

void Delete_info_File()
{
  if (LittleFS.exists("/info.dat"))
  {
    LittleFS.remove("/info.dat");
  }
  ESP.restart();
}

void Save_info_File(String wifi_ssid, String wifi_pass, String CORE_IOT_TOKEN, String CORE_IOT_SERVER, String CORE_IOT_PORT)
{
  Serial.println(wifi_ssid);
  Serial.println(wifi_pass);

  DynamicJsonDocument doc(4096);
  doc["WIFI_SSID"] = wifi_ssid;
  doc["WIFI_PASS"] = wifi_pass;
  doc["CORE_IOT_TOKEN"] = CORE_IOT_TOKEN;
  doc["CORE_IOT_SERVER"] = CORE_IOT_SERVER;
  doc["CORE_IOT_PORT"] = CORE_IOT_PORT;

  CoreConfig config;
  config.ssid = wifi_ssid;
  config.pass = wifi_pass;
  config.token = CORE_IOT_TOKEN;
  config.server = CORE_IOT_SERVER;
  config.port = CORE_IOT_PORT;
  updateCoreConfig(config);

  File configFile = LittleFS.open("/info.dat", "w");
  if (configFile)
  {
    serializeJson(doc, configFile);
    configFile.close();
  }
  else
  {
    Serial.println("Unable to save the configuration.");
  }
  ESP.restart();
};

bool check_info_File(bool check)
{
  if (!check)
  {
    if (!LittleFS.begin(true))
    {
      Serial.println("❌ Lỗi khởi động LittleFS!");
      return false;
    }
    Load_info_File();
  }
  
  CoreConfig config;
  const bool hasConfig = getLatestCoreConfig(config);
  if (!hasConfig || (config.ssid.isEmpty() && config.pass.isEmpty()))
  {
    if (!check)
    {
      startAP();
    }
    return false;
  }
  return true;
}