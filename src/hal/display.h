#pragma once
#include <Arduino.h>
#include "core/canvas.h"
class Display
{
public:
    Canvas *getCanvas() { return _canvas; }

    static constexpr int WIDTH = 320;
    static constexpr int HEIGHT = 240;

    void begin();
    void present();

private:
    uint16_t *_fb = nullptr;
    Canvas *_canvas = nullptr;
};
