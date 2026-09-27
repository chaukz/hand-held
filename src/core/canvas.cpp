#include "canvas.h"

void Canvas::clear(uint16_t color)
{
    if (_fb == nullptr)
    {
        return;
    }

    for (int i = 0; i < _w * _h; i++)
    {
        _fb[i] = color;
    }
}

void Canvas::drawPixel(int x, int y, uint16_t color)
{
    if (_fb == nullptr)
    {
        return;
    }
    if (x >= 0 && x < _w && y >= 0 && y < _h)
    {
        _fb[y * _w + x] = color;
    }
}

void Canvas::fillRect(int x, int y, int w, int h, uint16_t color)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            drawPixel(x + j, y + i, color);
        }
    }
}