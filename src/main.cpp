#include <Arduino.h>
#include "hal/pins.h"
#include "hal/input.h"
#include "hal/display.h"
#include "core/canvas.h"
#include "hal/audio.h"

Audio audio;
Display display;
Input input;

static constexpr uint32_t FRAME_MS = 33; // ~30 FPS
Canvas *canvas = nullptr;

void setup()
{
    Serial.begin(115200);
    input.begin();
    audio.begin();
    Serial.println("Handheld boot OK");
    display.begin();
    Canvas *current = display.getCanvas();
    if (current == nullptr)
    {
        Serial.println("Failed to get canvas from display!");
    }
    else
    {
        canvas = current;
    }
}

void loop()
{
    input.poll();
    InputEvent event;

    while (input.nextEvent(event))
    {
        if (event.type == InputType::Pressed)
        {
            Serial.printf("Button %d pressed\n", static_cast<int>(event.button));
            if (event.button == Button::A)
            {
                audio.playTone(880, 100); // Play a 880 Hz tone for 100 ms
            }
        }
    }
    audio.update();
    
    if (canvas == nullptr)
    {
        return;
    }
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
