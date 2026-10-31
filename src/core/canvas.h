#pragma once

#include <cstdint>

// Platform-independent RGB565 drawing surface. The caller owns the framebuffer.
class Canvas
{
    int _w;
    int _h;
    uint16_t *_fb;

public:
    Canvas(int w, int h, uint16_t *fb) : _w(w), _h(h), _fb(fb) {}

    void clear(uint16_t color);
    void drawPixel(int x, int y, uint16_t color);
    void fillRect(int x, int y, int w, int h, uint16_t color);
};
