#include "MenuManager.h"
#include "DisplayManager.h"
#include "RotaryEncoderManager.h"
#include "MqttManager.h"

enum MenuState { MAIN_MENU, SUB_MENU, ADJUST_BRIGHTNESS, ADJUST_COLOR, ADJUST_COLOR_TEMP };
MenuState menuState = MAIN_MENU;

int currentMenuIndex = 0;
String mainMenuItems[] = {"Light 1", "Light 2", "Light 3"};
String subMenuItems[] = {"Adjust Brightness", "Adjust Color", "Adjust Color Temperature", "Back"};
String currentSelection = "";
bool optionSelected = false;  // Tracks if an option is currently selected

int lastBrightness = -1;  // To store the last brightness value
int lastColor = -1;       // To store the last color value
int lastColorTemp = -1;   // To store the last color temperature value

void MenuManager::init() {
    DisplayManager::displayMenu(currentSelection, mainMenuItems, sizeof(mainMenuItems) / sizeof(mainMenuItems[0]), currentMenuIndex);
    Serial.println("Menu initialized.");
}

void MenuManager::handleInput() {
    int newPos = RotaryEncoderManager::getPosition();

    if (!optionSelected) {  // If no option is selected, rotating changes the menu
        if (menuState == MAIN_MENU) {
            currentMenuIndex = newPos % (sizeof(mainMenuItems) / sizeof(mainMenuItems[0]));
            DisplayManager::displayMenu(currentSelection, mainMenuItems, sizeof(mainMenuItems) / sizeof(mainMenuItems[0]), currentMenuIndex);
        } else if (menuState == SUB_MENU) {
            currentMenuIndex = newPos % (sizeof(subMenuItems) / sizeof(subMenuItems[0]));
            DisplayManager::displayMenu(currentSelection, subMenuItems, sizeof(subMenuItems) / sizeof(subMenuItems[0]), currentMenuIndex);
        }
    }

    if (RotaryEncoderManager::isButtonPressed()) {
        if (!optionSelected) {
            optionSelected = true;  // Mark the option as selected
            selectMenuItem();  // Enter the selected option
        } else {
            optionSelected = false;  // Unselect the option
            if (menuState == MAIN_MENU) {
                DisplayManager::displayMenu(currentSelection, mainMenuItems, sizeof(mainMenuItems) / sizeof(mainMenuItems[0]), currentMenuIndex);
            } else if (menuState == SUB_MENU) {
                DisplayManager::displayMenu(currentSelection, subMenuItems, sizeof(subMenuItems) / sizeof(subMenuItems[0]), currentMenuIndex);
            }
            Serial.println("Option unselected, returning to menu.");
        }
    }
}

void MenuManager::adjustBrightness() {
    int brightness = RotaryEncoderManager::getPosition();
    if (brightness != lastBrightness) {  // Only send if the brightness has changed
        MqttManager::sendMqttMessage("home/livingroom/light/brightness", brightness);
        Serial.print("Adjusted brightness to: ");
        Serial.println(brightness);
        lastBrightness = brightness;  // Update the last known brightness
    }
}

void MenuManager::adjustColor() {
    int color = RotaryEncoderManager::getPosition();
    if (color != lastColor) {  // Only send if the color has changed
        MqttManager::sendMqttMessage("home/livingroom/light/color", color);
        Serial.print("Adjusted color to: ");
        Serial.println(color);
        lastColor = color;  // Update the last known color
    }
}

void MenuManager::adjustColorTemp() {
    int colorTemp = RotaryEncoderManager::getPosition();
    if (colorTemp != lastColorTemp) {  // Only send if the color temperature has changed
        int mireds = map(colorTemp, 0, 100, 153, 500);  // Map encoder position to mireds
        MqttManager::sendMqttMessage("home/livingroom/light/color_temp", mireds);
        Serial.print("Adjusted color temperature to: ");
        Serial.println(mireds);
        lastColorTemp = colorTemp;  // Update the last known color temperature
    }
}

void MenuManager::selectMenuItem() {
    if (menuState == MAIN_MENU) {
        currentSelection = mainMenuItems[currentMenuIndex];
        menuState = SUB_MENU;
        DisplayManager::displayMenu(currentSelection, subMenuItems, sizeof(subMenuItems) / sizeof(subMenuItems[0]), currentMenuIndex);
        Serial.print("Selected: ");
        Serial.println(currentSelection);
    } else if (menuState == SUB_MENU) {
        if (subMenuItems[currentMenuIndex] == "Adjust Brightness") {
            menuState = ADJUST_BRIGHTNESS;
            adjustBrightness();
        } else if (subMenuItems[currentMenuIndex] == "Adjust Color") {
            menuState = ADJUST_COLOR;
            adjustColor();
        } else if (subMenuItems[currentMenuIndex] == "Adjust Color Temperature") {
            menuState = ADJUST_COLOR_TEMP;
            adjustColorTemp();
        } else if (subMenuItems[currentMenuIndex] == "Back") {
            menuState = MAIN_MENU;
            DisplayManager::displayMenu(currentSelection, mainMenuItems, sizeof(mainMenuItems) / sizeof(mainMenuItems[0]), currentMenuIndex);
            Serial.println("Going back to Main Menu.");
        }
    }
}
