#include "display.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

static TFT_eSPI tft;

void Display::begin()
{
    tft.init();
    tft.setRotation(1);
    _fb = (uint16_t *)malloc(Display::WIDTH * Display::HEIGHT * 2);
    if (_fb == nullptr)
        Serial.println("Failed to allocate framebuffer memory!");
    else
        _canvas = new Canvas(Display::WIDTH, Display::HEIGHT, _fb);
}

void Display::present()
{
    if (!_fb)
        return;

    constexpr int STRIP_H = 40;
    for (int y = 0; y < Display::HEIGHT; y += STRIP_H)
    {
        tft.pushImage(0, y, Display::WIDTH, STRIP_H, _fb + y * Display::WIDTH);
        yield();
    }
}
