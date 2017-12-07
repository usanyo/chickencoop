
#include <ESP8266WiFi.h>
#include <Button.h>

Button::~Button() {
}

Button::Button(u8 pinNumber, u8 pressedValue) {
    this->pressedValue = pressedValue;
    this->pinNumber = pinNumber;
}

void Button::setup() {
    pinMode(pinNumber, INPUT_PULLUP);
}

bool Button::isPressed() {
    return digitalRead(pinNumber) == pressedValue;
}

bool Button::isReleased() {
    return !isPressed();
}