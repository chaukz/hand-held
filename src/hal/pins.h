#pragma once

// SPI bus (prefixed to avoid collision with Arduino's own SCK/MOSI/MISO defines)
constexpr int PIN_SCK  = 12;
constexpr int PIN_MOSI = 11;
constexpr int PIN_MISO = 13;

// Display
constexpr int TFT_CS   = 10;
constexpr int TFT_DC   =  9;
constexpr int TFT_RST  =  8;

// SD card
constexpr int SD_CS    = 14;

// Buttons
constexpr int BTN_UP     =  4;
constexpr int BTN_DOWN   =  5;
constexpr int BTN_LEFT   =  6;
constexpr int BTN_RIGHT  =  7;
constexpr int BTN_A      = 15;
constexpr int BTN_B      = 16;
constexpr int BTN_START  = 17;
constexpr int BTN_SELECT = 18;

// Audio
constexpr int BUZZER   = 21;
