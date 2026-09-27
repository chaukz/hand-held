#pragma once
#include "core/input_event.h"

class Input
{
public:
    void begin();

    void poll();
    bool nextEvent(InputEvent &out);
    
};