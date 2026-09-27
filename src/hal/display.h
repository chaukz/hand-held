#pragma once
#include <Arduino.h>
#include "core/canvas.h"
class Display
{
    Canvas *_canvas = nullptr;

public:
    static constexpr int WIDTH = 320;
    static constexpr int HEIGHT = 240;

    void begin();
    void present();
    Canvas &getCanvas();

private:
    uint16_t *_fb = nullptr;
};