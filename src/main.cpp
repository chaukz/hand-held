#include <Arduino.h>
#include "hal/pins.h"

static constexpr uint32_t FRAME_MS = 33; // ~30 FPS

void setup() {
    Serial.begin(115200);
    Serial.println("Handheld boot OK");
}

void loop() {
    static uint32_t lastMs = 0;
    uint32_t now     = millis();
    uint32_t elapsed = now - lastMs;

    if (elapsed < FRAME_MS) return;
    lastMs = now;

    float dt = elapsed / 1000.0f;
    (void)dt; // used once drivers and apps are wired in

    // Stage 1: input.poll() → app.update(dt) → app.draw(canvas)
}
