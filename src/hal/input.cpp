#include "hal/input.h"
#include "core/input_event.h"
#include "hal/pins.h"
#include <Arduino.h>

static bool lastState[8] = {};
static uint32_t lastChangeTime[8] = {};

static InputEvent queue[16];
static int head = 0, tail = 0; // Head and tail indices for the queue

static const int PIN_MAP[] = {
    BTN_UP,
    BTN_DOWN,
    BTN_LEFT,
    BTN_RIGHT,
    BTN_A,
    BTN_B,
    BTN_START,
    BTN_SELECT};

void Input::begin()
{

    for (int i = 0; i < 8; i++)
    {
        pinMode(PIN_MAP[i], INPUT_PULLUP);
    }
}

void Input::poll()
{
    for (int i = 0; i < 8; i++)
    {
        bool pressed = digitalRead(PIN_MAP[i]) == LOW;
        if (pressed != lastState[i])
        {
            if (millis() - lastChangeTime[i] > 10)
            {
                queue[tail] = {static_cast<Button>(i), pressed ? InputType::Pressed : InputType::Released, millis()};
                tail = (tail + 1) % 16; // Move tail forward, wrap
                lastState[i] = pressed;
            }
            else
            {
                // Debounce: ignore changes that happen too quickly
                lastChangeTime[i] = millis();
            }
        }
    }
}

// Implementation for retrieving the next input event
bool Input::nextEvent(InputEvent &out)
{
    if (head == tail)
        return false; // Queue is empty
    out = queue[head];
    head = (head + 1) % 16; // Move head forward, wrap

    return true;
}
