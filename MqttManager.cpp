#include "MqttManager.h"
#include <WiFi.h>

// Define the static members of the class
WiFiClient MqttManager::espClient;
PubSubClient MqttManager::client(MqttManager::espClient);  // Properly define and initialize the client

const char* ssid = "Your-WiFi-SSID";               // Your WiFi SSID
const char* wifi_password = "Password"; // Your WiFi password

const char* mqtt_server = "ServerIP";    // Your MQTT server IP address
const char* mqtt_username = "username";      // Your MQTT username
const char* mqtt_password = "password";      // Your MQTT password
const int mqtt_port = xxxx;                   // Your MQTT port (usually 1883)

void MqttManager::init() {
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);  // Set the MQTT server and port
    reconnect();  // Attempt to connect to the MQTT broker
}

void MqttManager::setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void MqttManager::reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect with the given credentials
        if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
            Serial.println("connected");
            // Subscribe to any topics here if needed
            // client.subscribe("some/topic");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void MqttManager::loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

void MqttManager::sendMqttMessage(const String& topic, int value) {
    String message = String(value);
    client.publish(topic.c_str(), message.c_str());
    Serial.print("Published ");
    Serial.print(value);
    Serial.print(" to ");
    Serial.println(topic);
}
