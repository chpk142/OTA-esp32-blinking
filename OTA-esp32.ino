#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define LED_PIN 2 // Define the GPIO pin for the LED (use onboard LED for ESP32)

// Replace with your network credentials
const char* ssid = "your wfi name";
const char* password = "your wifi password";

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("Booting...");

  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure OTA
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  // Set OTA timeout to 30 seconds (30000 milliseconds)
  ArduinoOTA.setTimeout(30000);

  ArduinoOTA.begin();
  Serial.println("OTA Ready");
}

void loop() {
  // Handle OTA updates
  ArduinoOTA.handle();

  // Blink the LED
  digitalWrite(LED_PIN, HIGH);
  delay(2000);
  digitalWrite(LED_PIN, LOW);
  delay(2000);
}