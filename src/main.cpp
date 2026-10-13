#include <Arduino.h>
#include "hal/pins.h"
#include "hal/input.h"
#include "hal/display.h"
#include "core/canvas.h"
#include "hal/audio.h"

Audio audio;
Display display;
Input input;

static constexpr uint32_t FIXED_STEP_US = 1000000UL / 30UL;
static constexpr uint8_t MAX_CATCH_UP_STEPS = 5;
static uint32_t previousLoopUs = 0;
static uint32_t accumulatorUs = 0;
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
    previousLoopUs = micros();
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

    const uint32_t nowUs = micros();
    const uint32_t elapsedUs = nowUs - previousLoopUs;
    previousLoopUs = nowUs;
    accumulatorUs += elapsedUs;

    if (canvas == nullptr)
    {
        return;
    }

    uint8_t steps = 0;
    while (accumulatorUs >= FIXED_STEP_US && steps < MAX_CATCH_UP_STEPS)
    {
        accumulatorUs -= FIXED_STEP_US;
        ++steps;
    }

    if (steps == 0)
        return;

    // Drop excess whole steps after a long stall; retain only the fractional remainder.
    if (accumulatorUs >= FIXED_STEP_US)
        accumulatorUs %= FIXED_STEP_US;

    canvas->clear(0x0000);             // Clear the canvas to black
    canvas->drawPixel(10, 10, 0xFFFF); // Draw a white pixel at (10, 10)
    display.present();                 // Present the canvas to the display
}
