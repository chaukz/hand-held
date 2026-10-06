#include "audio.h"
#include <Arduino.h>
#include "hal/pins.h"

void Audio::begin()
{
    _toneActive = false;
    pinMode(BUZZER, OUTPUT);
    noTone(BUZZER);
}

void Audio::update()
{
    if (_toneActive && static_cast<uint32_t>(millis() - _toneStartedAt) >= _toneDurationsMs)
    {
        noTone(BUZZER);
        _toneActive = false;
    }
}

void Audio::playTone(uint32_t frequencyHz, uint32_t durationMs)
{
    if (frequencyHz == 0 || durationMs == 0)
    {
        noTone(BUZZER);
        _toneActive = false;
        return;
    }
    tone(BUZZER, frequencyHz);
    _toneActive = true;
    _toneStartedAt = millis();
    _toneDurationsMs = durationMs;
}