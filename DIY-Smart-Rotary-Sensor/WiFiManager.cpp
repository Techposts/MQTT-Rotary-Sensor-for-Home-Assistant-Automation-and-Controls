#include "WiFiManager.h"

// Define static variables
unsigned long WiFiManager::apStartTime = 0;
const unsigned long WiFiManager::apTimeout = 300000;  // 5 minutes

WebServer WiFiManager::server(80);
const char* WiFiManager::apSSID = "ESP32_Config";
const char* WiFiManager::apPassword = "12345678";
bool WiFiManager::apMode = false;

void WiFiManager::init() {
    WiFi.mode(WIFI_STA);  // Start in station mode
    connectToWiFi();
}

void WiFiManager::connectToWiFi() {
    WiFi.begin();

    unsigned long startAttemptTime = millis();

    // Keep trying to connect for 120 seconds (120000 milliseconds)
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 120000) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nFailed to connect to Wi-Fi, starting AP mode.");
        startAP();
    } else {
        Serial.println("\nConnected to Wi-Fi");
    }
}

void WiFiManager::startAP() {
    apMode = true;
    WiFi.mode(WIFI_AP);  // Switch to AP mode
    WiFi.softAP(apSSID, apPassword);
    apStartTime = millis();  // Record the time AP mode started
    Serial.println("AP mode started");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    
    setupWebServer();
}

void WiFiManager::setupWebServer() {
    server.on("/", handleRoot);
    server.on("/scan", handleScanNetworks);  // Correctly map the /scan URL to the handleScanNetworks function
    server.on("/save", handleSave);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("Web server started");
}

void WiFiManager::handleWiFi() {
    if (apMode) {
        server.handleClient();
        
        // Check if AP mode has been running for more than the timeout period
        if (millis() - apStartTime > apTimeout) {
            Serial.println("AP mode timeout, retrying Wi-Fi connection...");
            WiFi.softAPdisconnect(true);  // Stop AP mode
            apMode = false;  // Reset AP mode flag
            connectToWiFi();  // Retry connecting to Wi-Fi
        }
    } else {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Wi-Fi disconnected, attempting to reconnect...");
            connectToWiFi();
        }
    }
}

void WiFiManager::handleRoot() {
    String page = "<html><head><title>ESP32 Wi-Fi Config</title>";
    page += "<style>body { font-family: Arial, sans-serif; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; background-color: #f0f0f0; }";
    page += "h1 { color: #333; }";
    page += ".container { width: 80%; max-width: 600px; padding: 20px; background: white; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); border-radius: 8px; }";
    page += "input[type='submit'] { background-color: #4CAF50; color: white; padding: 10px 15px; border: none; border-radius: 5px; cursor: pointer; }";
    page += "input[type='submit']:hover { background-color: #45a049; }";
    page += "</style></head><body><div class='container'>";
    page += "<h1>ESP32 Wi-Fi Config</h1>";
    page += "<form action='/scan' method='post'><input type='submit' value='Scan for Networks'></form>";
    page += "</div></body></html>";
    server.send(200, "text/html", page);
}

void WiFiManager::handleScanNetworks() {
    int n = WiFi.scanNetworks();  // Perform network scan
    String page = "<html><head><title>Select Wi-Fi Network</title>";
    page += "<style>body { font-family: Arial, sans-serif; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; background-color: #f0f0f0; }";
    page += "h1 { color: #333; }";
    page += ".container { width: 80%; max-width: 600px; padding: 20px; background: white; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); border-radius: 8px; }";
    page += "input[type='radio'] { margin-right: 10px; }";
    page += "input[type='submit'] { background-color: #4CAF50; color: white; padding: 10px 15px; border: none; border-radius: 5px; cursor: pointer; }";
    page += "input[type='submit']:hover { background-color: #45a049; }";
    page += "</style></head><body><div class='container'>";
    page += "<h1>Select Wi-Fi Network</h1><form action='/save' method='post'>";
    
    if (n == 0) {
        page += "No networks found.<br>";
    } else {
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            page += "<input type='radio' name='ssid' value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i);
            page += " (" + String(WiFi.RSSI(i)) + " dBm)";
            page += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " [Open]" : " [Secured]";
            page += "<br>";
        }
    }
    
    page += "<br>Password: <input type='password' name='password'><br><br>";
    page += "<input type='submit' value='Connect'>";
    page += "</form></div></body></html>";
    server.send(200, "text/html", page);
}

void WiFiManager::handleSave() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    Serial.println("Saving new Wi-Fi credentials...");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(password);

    // Clear old credentials
    WiFi.disconnect(true);

    // Begin with new credentials
    WiFi.begin(ssid.c_str(), password.c_str());

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to new Wi-Fi");
        server.send(200, "text/html", "Connected to new Wi-Fi. Please disconnect and reconnect to the new network.");
        delay(1000);
        ESP.restart();  // Restart to apply new settings
    } else {
        Serial.println("\nFailed to connect with new credentials");
        server.send(200, "text/html", "Failed to connect with new credentials. Please try again.");
    }
}

void WiFiManager::handleNotFound() {
    server.send(404, "text/html", "Page not found");
}
