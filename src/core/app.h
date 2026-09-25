#pragma once
#include "input_event.h"

class Canvas; // defined in hal/display.h — Stage 1

class App {
public:
    virtual ~App() = default;

    virtual const char* name() const = 0;
    virtual void onStart() = 0;
    virtual void onStop()  = 0;
    virtual void onInput(const InputEvent& e) = 0;
    virtual void update(float dt) = 0;  // dt in seconds
    virtual void draw(Canvas& c)  = 0;
};
