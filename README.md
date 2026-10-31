# hand-held

**A C++ mini-OS and handheld console for the ESP32-S3, with a Linux desktop simulator.**

The goal is a portable application runtime with a launcher, games, input, graphics, audio, and eventually SD-card storage and CHIP-8 support. The ESP32-S3 firmware is the mini-OS; the desktop build is a development simulator, **not Linux running on the microcontroller**.

> **Status (31 October 2026):** Stage 0 is documented as complete; Stage 1 is in progress. The desktop simulator is an early rendering scaffold. Builds, hardware behavior, and GitHub Actions results have not yet been verified. No physical parts are required to develop the desktop side.

## Current progress

| Area | Status | Notes |
| --- | --- | --- |
| Wokwi circuit | Documented complete | ESP32-S3, ILI9341, microSD, eight buttons, buzzer |
| GPIO mapping | Implemented | `src/hal/pins.h` |
| Display and RGB565 Canvas | Implemented | 320×240 framebuffer, strip-based SPI transfer |
| Button input and audio | Implemented | Input event polling and buzzer driver |
| Fixed-step timing | Partial | 30 Hz accumulator exists; no app update integration yet |
| Desktop simulator | Scaffold | SDL2 window and Canvas drawing demo; no shared app runtime yet |
| Native Canvas tests | Added, unverified | CTest target and GitHub Actions workflow |
| Test-screen / Snake | Not implemented | Next Stage 1 applications |
| Launcher, storage, FreeRTOS, CHIP-8 | Planned | Later stages |
| Physical device | Not validated | Hardware purchase and testing deferred |

The current ESP32 loop renders a static pixel each frame. A full-screen transfer on every frame can make Wokwi slow; performance improvements are planned, not yet benchmarked.

## Quick start

### Desktop simulator (Linux)

Dependencies: CMake, C++17 compiler, `pkg-config`, and SDL2 development files.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/handheld-desktop
```

The demo opens a 640×480 window showing a scaled 320×240 RGB565 framebuffer. Press **Escape** or close the window to exit. It does **not** yet emulate buttons, games, or ESP32 peripherals.

Run the Canvas tests:

```bash
ctest --test-dir build --output-on-failure
```

On NixOS, make these dependencies available in a development shell before running CMake. The repository does not yet include a `flake.nix` or `shell.nix`.

### ESP32-S3 firmware (PlatformIO + Arduino)

```bash
pio run -e esp32-s3-devkitc-1
```

To run Wokwi, build the firmware first and launch the Wokwi VS Code extension using `diagram.json` and `wokwi.toml`. The latter points at the PlatformIO `.bin` and `.elf` outputs. The existing firmware uses `TFT_eSPI`; the desktop target uses SDL2. **Do not flash the desktop executable to the ESP32.**

## Architecture

```text
                 Applications (planned)
          Snake · Launcher · Life · CHIP-8
                         |
               Shared core and Canvas
                         |
              Platform-specific adapters
                  /             \
         Linux / SDL2       ESP32-S3 / Arduino
         window, keyboard   TFT, buttons, buzzer, SD
```

The separation is **in progress**, not complete: Canvas is portable, but the ESP32 input, display and audio implementations still depend on Arduino.

### Repository layout

```text
CMakeLists.txt                Desktop build and native tests
desktop/main.cpp              SDL2 rendering demo
tests/canvas_test.cpp         Canvas drawing tests
.github/workflows/desktop.yml Desktop CI
platformio.ini                ESP32-S3 Arduino build
wokwi.toml                    Wokwi firmware paths
diagram.json                  Wokwi circuit
src/main.cpp                  ESP32 boot and loop
src/core/canvas.{h,cpp}       Shared RGB565 drawing API
src/hal/                      ESP32 display, input, audio, pins
```

## Roadmap

### Stage 0 — Circuit
- [x] Create ESP32-S3 Wokwi circuit
- [x] Wire display, SD, eight buttons, buzzer
- [x] Document serial boot output

### Stage 1 — Playable first app
- [x] Centralize pins
- [x] Add display and framebuffer/Canvas
- [x] Add button events and buzzer
- [x] Add fixed-step timing accumulator
- [ ] Integrate app update/draw into the fixed-step loop
- [ ] Implement button test-screen app
- [ ] Implement playable Snake
- [ ] Verify stable rendering and timing

### Desktop development track
- [x] Make Canvas independent of Arduino
- [x] Add SDL2 rendering scaffold and CMake target
- [x] Add native Canvas tests and CI configuration
- [ ] Confirm desktop CI passes
- [ ] Map keyboard to shared input events
- [ ] Extract reusable app lifecycle and game logic
- [ ] Run test-screen and Snake on desktop
- [ ] Reduce unnecessary full-screen transfers in Wokwi

### Stage 2 — Mini-OS launcher
- [ ] AppManager registration, lifecycle, switching
- [ ] Scrollable launcher and Start+Select shortcut
- [ ] Game of Life and a third app

### Stage 3 — Storage and settings
- [ ] Mount SD and browse files
- [ ] Persist high scores and settings

### Stage 4 — FreeRTOS
- [ ] Separate UI, input and audio tasks
- [ ] Queues and performance diagnostics

### Stage 5 — CHIP-8
- [ ] Interpreter and graphics/input integration
- [ ] Load ROMs from storage and launch from file browser

### Hardware validation (later)
- [ ] Test display SPI speed, buttons, memory and sound on a real ESP32-S3
- [ ] Validate power and battery circuitry before portable use

## Wokwi wiring (v0.1)

Screen and SD share SPI. Buttons use pull-ups (pressed = LOW).

| Signal | GPIO | Signal | GPIO |
| --- | --- | --- | --- |
| SPI SCK | 12 | SPI MOSI | 11 |
| SPI MISO | 13 | TFT CS | 10 |
| TFT DC | 9 | TFT RST | 8 |
| SD CS | 14 | Buzzer | 21 |
| Up | 4 | Down | 5 |
| Left | 6 | Right | 7 |
| A | 15 | B | 16 |
| Start | 17 | Select | 18 |

TFT backlight is tied to 3V3 in the simulated circuit. Recheck pin compatibility against the exact board purchased.

## Hardware plan

ESP32-S3 with PSRAM, compatible SPI TFT, tactile buttons, microSD, buzzer, and eventually a protected battery/charging system. Wokwi cannot validate actual electrical behavior, power consumption, or physical SPI throughput.

## Development workflow

Work is taking place on `feature/desktop-simulator` while `main` remains unchanged. Changes should be incremental and reviewable. CI and physical-device validation must be distinguished from merely writing or committing code.

**Next objective:** get a passing desktop build, wire up shared input and app lifecycle, and make Snake playable without depending on Wokwi.
