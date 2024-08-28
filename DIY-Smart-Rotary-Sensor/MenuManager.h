#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <Arduino.h>

class MenuManager {
public:
    static void init();
    static void handleInput();
    static void adjustBrightness();
    static void adjustColor();
    static void adjustColorTemp();  // Add this line
    static void selectMenuItem();
};

#endif
