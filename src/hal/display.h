#pragma once
#include <Arduino.h>

class Display
{
public:
    static constexpr int WIDTH = 320;
    static constexpr int HEIGHT = 240;

    void begin();
    void clear(uint16_t color);
    void drawPixel(int x, int y, uint16_t color);
    void present();

private:
    uint16_t *_fb = nullptr;
};