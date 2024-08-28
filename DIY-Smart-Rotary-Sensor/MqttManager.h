#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>

class MqttManager {
public:
    static void init();
    static void reconnect();
    static void loop();
    static void sendMqttMessage(const String& topic, int value);
    
private:
    static WiFiClient espClient;
    static PubSubClient client;  // Declaration of the client object
    static void setup_wifi();
};

#endif
