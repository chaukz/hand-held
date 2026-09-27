#include <Arduino.h>
#include "hal/pins.h"
#include "hal/input.h"
#include "hal/display.h"
#include "core/canvas.h"

Display display;

static constexpr uint32_t FRAME_MS = 33; // ~30 FPS
Canvas *canvas = nullptr;

void setup()
{
    Serial.begin(115200);
    Serial.println("Handheld boot OK");
    display.begin();
    canvas = &display.getCanvas();
}

void loop()
{
    static uint32_t lastMs = 0;
    uint32_t now = millis();
    uint32_t elapsed = now - lastMs;

    if (elapsed < FRAME_MS)
        return;
    lastMs = now;

    float dt = elapsed / 1000.0f;
    (void)dt; // used once drivers and apps are wired in

    canvas->clear(0x0000);             // Clear the canvas to black
    canvas->drawPixel(10, 10, 0xFFFF); // Draw a white pixel at (10, 10)
    display.present();                 // Present the canvas to the display
}
