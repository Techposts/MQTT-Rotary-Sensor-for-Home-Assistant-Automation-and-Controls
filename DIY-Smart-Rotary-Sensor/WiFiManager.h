#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

class WiFiManager {
public:
    static void init();
    static void handleWiFi();
    
private:
    static void connectToWiFi();          // Declare the connectToWiFi function
    static void startAP();
    static void setupWebServer();
    static void handleRoot();
    static void handleScanNetworks();     // Declare the handleScanNetworks function
    static void handleSave();
    static void handleNotFound();

    static WebServer server;
    static const char* apSSID;
    static const char* apPassword;
    static bool apMode;
    static unsigned long apStartTime;
    static const unsigned long apTimeout;
};

#endif
