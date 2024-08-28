#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define I2C pins for the OLED display
#define OLED_SDA_PIN 8    // Replace with your actual SDA pin number
#define OLED_SCL_PIN 9    // Replace with your actual SCL pin number

// Define display dimensions
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing Arduino reset pin)

// Define the I2C address for the OLED display
#define SSD1306_I2C_ADDRESS 0x3C  // Common I2C address for SSD1306 displays. Use 0x3D if 0x3C doesn't work.

class DisplayManager {
public:
    static void init();
    static void displayMenu(const String& currentItem, const String menuItems[], int menuSize, int currentIndex);
    static void displayMessage(const String& message);
private:
    static Adafruit_SSD1306 display;
};

#endif
