#pragma once
#include "core/input_event.h"

class Input {
    public:
    void begin() {
        // Implementation for beginning input processing
    }
    void poll() {
        // Implementation for polling input
    }
    bool nextEvent(InputEvent& out) {
        // Implementation for retrieving the next input event
        return false; // Placeholder return value
    }
};