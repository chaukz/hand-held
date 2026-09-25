# Handheld + Mini-OS (working name)

A DIY handheld console built on an **ESP32-S3**, prototyped entirely in **Wokwi** first, with a custom **mini-OS** written in C++. Real hardware comes later; the firmware is designed so the same code runs on both.

> The OS is not installed separately. It _is_ the firmware: drivers, services, a launcher, and apps compiled into one image and flashed to the chip. The handheld is usable from Stage 1 onward, and the OS grows around the first app.

---

## Goals

- Learn real embedded C++: drivers, timing, interrupts, memory limits, RTOS tasks.
- Build a clean layered architecture where apps never touch hardware directly.
- End with a device that runs multiple apps, saves data to SD, and loads CHIP-8 games from files.
- Keep everything simulatable in Wokwi until parts are bought.

## Non-goals (for now)

- Linux. The ESP32-S3 has no MMU and too little RAM for a practical Linux system. A Linux handheld would be a separate project on a Pi Zero 2 W or Luckfox Pico.
- Wi-Fi features, touchscreens, or colour audio. These can come after Stage 5.

---

## Hardware (Wokwi)

| Part                      | Wokwi type                 | Purpose                    |
| ------------------------- | -------------------------- | -------------------------- |
| ESP32-S3 DevKitC-1        | `board-esp32-s3-devkitc-1` | Main MCU                   |
| ILI9341 320×240 TFT (SPI) | `wokwi-ili9341`            | Screen                     |
| microSD card (SPI)        | `wokwi-microsd-card`       | Storage (FAT16 in sim)     |
| 8× pushbutton             | `wokwi-pushbutton`         | D-pad, A, B, Start, Select |
| Buzzer                    | `wokwi-buzzer`             | Sound                      |

### Pin map (v0.1)

Screen and SD card share one SPI bus with separate chip-select pins. Buttons use `INPUT_PULLUP` and connect to GND, so **pressed = LOW**.

Pins avoided on purpose: 0, 3, 45, 46 (strapping), 19, 20 (USB), 26 to 37 (flash/PSRAM on N16R8 boards).

| Signal     | GPIO | Notes                       |
| ---------- | ---- | --------------------------- |
| SPI SCK    | 12   | Shared                      |
| SPI MOSI   | 11   | Shared                      |
| SPI MISO   | 13   | Shared (SD needs it)        |
| TFT CS     | 10   |                             |
| TFT DC     | 9    |                             |
| TFT RST    | 8    |                             |
| TFT LED    | 3V3  | Backlight always on for now |
| SD CS      | 14   |                             |
| BTN UP     | 4    |                             |
| BTN DOWN   | 5    |                             |
| BTN LEFT   | 6    |                             |
| BTN RIGHT  | 7    |                             |
| BTN A      | 15   |                             |
| BTN B      | 16   |                             |
| BTN START  | 17   |                             |
| BTN SELECT | 18   |                             |
| BUZZER     | 21   | PWM via `ledc` / `tone()`   |

All pin numbers live in **one file** (`src/hal/pins.h`) so moving to real hardware is a one-file change.

---

## Architecture

```
┌──────────────────────────────────────────────┐
│  Apps          Snake · Life · Calc · CHIP-8  │
├──────────────────────────────────────────────┤
│  System UI     Launcher · Settings · Files   │
├──────────────────────────────────────────────┤
│  Core          AppManager · EventQueue ·     │
│                Timing · (later) FreeRTOS     │
├──────────────────────────────────────────────┤
│  HAL           Display · Input · Audio ·     │
│                Storage · pins.h              │
├──────────────────────────────────────────────┤
│  Hardware      ESP32-S3 + TFT + SD + buttons │
└──────────────────────────────────────────────┘
```

### Rules

1. **Each layer only talks to the layer directly below it.** Apps call `Canvas` and receive `InputEvent`s; they never call `digitalRead()` or SPI functions.
2. **No `delay()` in the main loop.** Use a fixed-timestep loop driven by `millis()` / `micros()`.
3. **Pins are only defined in `pins.h`.**
4. **Every app implements `App`.** The launcher only knows about the interface.
5. **Know your memory.** A full 320×240 16-bit framebuffer is 150 KB. Allocate it once at boot, check that the allocation succeeded, and fall back to strip rendering (e.g. 320×40 chunks) if it didn't.

### Core interfaces (draft)

```cpp
// core/input_event.h
enum class Button : uint8_t { Up, Down, Left, Right, A, B, Start, Select };
enum class InputType : uint8_t { Pressed, Released, Held };

struct InputEvent {
    Button    button;
    InputType type;
    uint32_t  timestampMs;
};
```

```cpp
// core/app.h
class Canvas;  // from hal/display.h

class App {
public:
    virtual const char* name() = 0;
    virtual void onStart() {}
    virtual void onStop() {}
    virtual void onInput(const InputEvent& e) = 0;
    virtual void update(float dt) = 0;       // dt in seconds
    virtual void draw(Canvas& c) = 0;
    virtual ~App() = default;
};
```

---

## Project layout

Set up for PlatformIO + the Wokwi VS Code extension (works on NixOS). The same files can be pasted into tabs on wokwi.com.

```
handheld/
├── README.md
├── platformio.ini
├── wokwi.toml
├── diagram.json            # Wokwi circuit
├── src/
│   ├── main.cpp            # boot + main loop only
│   ├── hal/
│   │   ├── pins.h
│   │   ├── display.h/.cpp  # ILI9341 + Canvas + framebuffer
│   │   ├── input.h/.cpp    # debouncing -> InputEvents
│   │   ├── audio.h/.cpp    # buzzer tones
│   │   └── storage.h/.cpp  # SD card (Stage 3)
│   ├── core/
│   │   ├── app.h
│   │   ├── input_event.h
│   │   ├── event_queue.h/.cpp
│   │   └── app_manager.h/.cpp  # (Stage 2)
│   └── apps/
│       ├── test_screen/    # Stage 1
│       ├── snake/          # Stage 1
│       ├── launcher/       # Stage 2
│       ├── life/           # Stage 2 (port of Game of Life)
│       └── chip8/          # Stage 5
└── docs/
    └── notes.md            # decisions, bugs, measurements
```

---

## Roadmap

### Stage 0: Circuit

- [x] Create the Wokwi project with the ESP32-S3
- [x] Wire the ILI9341, SD card, 8 buttons and buzzer per the pin map
- [x] Serial monitor prints a boot message

**Done when:** the circuit runs and prints to serial.

### Stage 1: One program (the handheld is usable)

- [ ] `pins.h` with every pin
- [ ] Display driver: fill screen, pixels, rectangles, text
- [ ] Framebuffer + `Canvas` class, push full frame to screen
- [ ] Input driver: debounced buttons → `InputEvent` queue
- [ ] Audio: beep on button press
- [ ] Fixed-timestep main loop (target 30 FPS)
- [ ] Test-screen app: shows which buttons are pressed
- [ ] Snake as a standalone `App`

**Done when:** Snake is playable with no flicker and no `delay()` calls.

### Stage 2: The OS appears

- [ ] `AppManager`: register apps, start/stop, switch
- [ ] Launcher app with a scrollable menu
- [ ] Start+Select returns to the launcher from any app
- [ ] Port Game of Life as an app
- [ ] A third app (calculator or stopwatch)

**Done when:** you can move between at least three apps from a menu.

### Stage 3: Storage + settings

- [ ] Mount the SD card, list files
- [ ] File browser app
- [ ] Save/load high scores
- [ ] Settings app (volume, brightness placeholder), saved to SD

**Done when:** settings and scores survive a restart.

### Stage 4: FreeRTOS

- [ ] Separate tasks: input, audio, UI/app
- [ ] Queues between tasks instead of shared globals
- [ ] Measure frame time and free heap; show them on a debug overlay

**Done when:** sound plays without stuttering the UI.

### Stage 5: Loading programs

- [ ] CHIP-8 interpreter as an app
- [ ] Load ROMs from SD (in Wokwi, store them as hex text files, since uploading binary files to the sim SD card is a paid feature)
- [ ] ROM picker in the file browser

**Done when:** a CHIP-8 game loaded from the card is playable.

### Stretch

- [ ] Tiny shell over serial (`ls`, `run snake`, `free`)
- [ ] Custom PCB in KiCad + 3D-printed case
- [ ] Wi-Fi app (clock sync, high-score upload)

---

## Real hardware (later)

Rough list, check local prices before buying:

- ESP32-S3 dev board with PSRAM (N16R8)
- 2.4" or 2.8" ILI9341/ST7789 SPI TFT (many include an SD slot)
- Tactile buttons, slide switch for power
- Passive buzzer, or MAX98357A I2S amp + small speaker
- LiPo cell + charger board **with protection circuitry**
- Perfboard first, then a custom PCB

Battery safety: use protected cells, avoid punctures and shorts, and don't leave it charging unattended until the power circuit is proven.

---

## Wokwi limitations to remember

- The sim is great for logic, timing and UI; it can't show real SPI speed, power draw, or button bounce accurately. Re-test all of these on hardware.
- Don't rely on PSRAM in the sim. Design to fit in internal RAM.
- The simulated SD card is formatted FAT16 and gets your project files copied onto it.

---

## Log

| Date       | Note                                                                                              |
| ---------- | ------------------------------------------------------------------------------------------------- |
| 2026-09-25 | Project started. ESP32-S3 chosen over Pico for dual core + better Wokwi support for display work. |
