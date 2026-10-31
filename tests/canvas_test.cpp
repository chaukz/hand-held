#include "core/canvas.h"
#include <cassert>
#include <cstdint>
#include <vector>

int main() {
    std::vector<uint16_t> pixels(8 * 6, 0);
    Canvas canvas(8, 6, pixels.data());
    canvas.clear(0x1234);
    for (auto pixel : pixels) assert(pixel == 0x1234);
    canvas.drawPixel(2, 3, 0xFFFF);
    assert(pixels[3 * 8 + 2] == 0xFFFF);
    canvas.drawPixel(-1, 0, 0x0000);
    canvas.drawPixel(8, 0, 0x0000);
    canvas.fillRect(6, 4, 4, 4, 0x07E0);
    assert(pixels[4 * 8 + 6] == 0x07E0);
    assert(pixels[5 * 8 + 7] == 0x07E0);
    assert(pixels[0] == 0x1234);
}
