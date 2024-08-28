#ifndef ROTARY_ENCODER_MANAGER_H
#define ROTARY_ENCODER_MANAGER_H

#include <RotaryEncoder.h>

// Define the GPIO pins for the rotary encoder
#define ROTARY_ENCODER_CLK_PIN 2   // Replace with your actual CLK pin number
#define ROTARY_ENCODER_DT_PIN  3   // Replace with your actual DT pin number
#define ROTARY_ENCODER_SW_PIN  4   // Replace with your actual SW pin number

class RotaryEncoderManager {
public:
    static void init();
    static void tick();
    static int getPosition();
    static void setPosition(int position);
    static bool isButtonPressed();
private:
    static RotaryEncoder encoder;
};

#endif
