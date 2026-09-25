#pragma once
#include <cstdint>

enum class Button : uint8_t {
    Up, Down, Left, Right,
    A, B, Start, Select
};

enum class InputType : uint8_t {
    Pressed,
    Released,
    Held
};

struct InputEvent {
    Button    button;
    InputType type;
    uint32_t  timestampMs;
};
