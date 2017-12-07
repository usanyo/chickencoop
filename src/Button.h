#ifndef BUTTON_H
#define BUTTON_H

#include <ESP8266WiFi.h>

class Button {
    public:
        Button(u8 pinNumber, u8 pressedValue);
        virtual ~Button();
        bool isPressed();
        bool isReleased();
        void setup();
    private:
        u8 pressedValue;
        u8 pinNumber;
};

#endif