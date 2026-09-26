#include "display.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

static TFT_eSPI tft;
static uint16_t *framebuffer = nullptr;

void Display::begin()
{
    tft.init();
    tft.setRotation(1);
    framebuffer = (uint16_t *)malloc(Display::WIDTH * Display::HEIGHT * 2);
    if (framebuffer == nullptr)
        Serial.println("Failed to allocate framebuffer memory!");
}

void Display::clear(uint16_t color)
{

    if (framebuffer)
    {
        for (int i = 0; i < Display::WIDTH * Display::HEIGHT; ++i)
        {
            framebuffer[i] = color;
        }
    }
}

void Display::drawPixel(int x, int y, uint16_t color)
{
    if (framebuffer && x >= 0 && x < Display::WIDTH && y >= 0 && y < Display::HEIGHT)
    {
        framebuffer[y * Display::WIDTH + x] = color;
    }
}

void Display::present()
{
    if (framebuffer)
    {
        tft.pushImage(0, 0, Display::WIDTH, Display::HEIGHT, framebuffer);
    }
}
