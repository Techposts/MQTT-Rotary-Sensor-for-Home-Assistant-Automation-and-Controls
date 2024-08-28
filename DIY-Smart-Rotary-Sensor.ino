#include "DisplayManager.h"
#include "RotaryEncoderManager.h"
#include "MqttManager.h"
#include "MenuManager.h"
#include "WiFiManager.h"

void setup() {
    Serial.begin(115200);
    WiFiManager::init();

    if (WiFi.status() == WL_CONNECTED) {
        MqttManager::init();            // Initialize MQTT only if Wi-Fi is connected
    }

    DisplayManager::init();
    RotaryEncoderManager::init();
    MenuManager::init();
}

void loop() {
    WiFiManager::handleWiFi();

    if (WiFi.status() == WL_CONNECTED) {
        MqttManager::loop();            // Handle MQTT only if Wi-Fi is connected
    }

    RotaryEncoderManager::tick();
    MenuManager::handleInput();
}
