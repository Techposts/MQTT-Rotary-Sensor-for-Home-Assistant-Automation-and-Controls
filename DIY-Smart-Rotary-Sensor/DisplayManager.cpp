#include "DisplayManager.h"

// Initialize the display with the defined screen width, height, and I2C address
Adafruit_SSD1306 DisplayManager::display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void DisplayManager::init() {
    // Initialize I2C communication for the OLED display
    Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

    if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Loop forever if the display initialization fails
    }

    display.clearDisplay();
    display.display();
}

void DisplayManager::displayMenu(const String& currentItem, const String menuItems[], int menuSize, int currentIndex) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
  
    for (int i = 0; i < menuSize; i++) {
        if (i == currentIndex) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Highlight the selected item
        } else {
            display.setTextColor(SSD1306_WHITE);
        }
        display.setCursor(0, i * 10);
        display.println(menuItems[i]);
    }
    display.display();
}

void DisplayManager::displayMessage(const String& message) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);  // Set the cursor position to the top-left corner
    display.println(message);  // Display the message
    display.display();  // Render the message on the screen
}
