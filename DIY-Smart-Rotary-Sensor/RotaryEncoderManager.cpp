#include "RotaryEncoderManager.h"

RotaryEncoder RotaryEncoderManager::encoder(ROTARY_ENCODER_CLK_PIN, ROTARY_ENCODER_DT_PIN);
int lastPosition = 0;  // Track the last known position

void RotaryEncoderManager::init() {
    pinMode(ROTARY_ENCODER_SW_PIN, INPUT_PULLUP);
    Serial.println("Rotary Encoder initialized.");
    lastPosition = encoder.getPosition();  // Initialize the last position
}

void RotaryEncoderManager::tick() {
    encoder.tick();  // Updates the encoder state
    int position = encoder.getPosition();

    // Log the position change if any
    if (position != lastPosition) {
        Serial.print("Rotary Encoder Position changed to: ");
        Serial.println(position);
    }

    lastPosition = position;  // Update last known position
}

int RotaryEncoderManager::getPosition() {
    return encoder.getPosition();
}

void RotaryEncoderManager::setPosition(int position) {
    encoder.setPosition(position);
    Serial.print("Set Rotary Encoder Position: ");
    Serial.println(position);
}

bool RotaryEncoderManager::isButtonPressed() {
    static uint32_t lastButtonPress = 0;
    if (digitalRead(ROTARY_ENCODER_SW_PIN) == LOW) {
        if (millis() - lastButtonPress > 500) {
            lastButtonPress = millis();
            Serial.println("Rotary Encoder Button Pressed.");
            return true;
        }
    }
    return false;
}
