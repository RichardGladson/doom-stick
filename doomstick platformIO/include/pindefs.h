/*
    doomstick - subset of doomBOX by Richard Gladson
    Adapted for ESP32-C3 SuperMini + 0.96" OLED + 2 buttons
    https://github.com/RichardGladson/doomBOX
    Copyright (c) 2026 Richard Gladson

    Licensed under the GNU General Public License v3.0
    https://www.gnu.org/licenses/gpl-3.0.html

    SPDX-License-Identifier: GPL-3.0-only
*/
#ifndef PINDEFS_H
#define PINDEFS_H

// doomstick: ESP32-C3 SuperMini
// 2-button interface: MOVE (cycle) + SELECT
// Aliases preserve original module code that expects 5 buttons.

#define BUTTON_PIN_MOVE     3   // Cycle / Up / Down
#define BUTTON_PIN_SELECT   10  // Select / Confirm / Back (long press in some UIs)

// Compatibility aliases used throughout original modules
#define BUTTON_PIN_UP       BUTTON_PIN_MOVE
#define BUTTON_PIN_DOWN     BUTTON_PIN_MOVE
#define BUTTON_PIN_CENTER   BUTTON_PIN_SELECT
#define BUTTON_PIN_LEFT     BUTTON_PIN_SELECT   // Back / Left
#define BUTTON_PIN_RIGHT    BUTTON_PIN_SELECT   // Select / Right

// No NeoPixel or RFID on doomstick
#define NEOPIXEL_PIN        -1

// I2C OLED (remapped away from strapping pins 8/9)
// Wire.begin(SDA, SCL) called in setup
#define OLED_SDA            6
#define OLED_SCL            7

#endif // PINDEFS_H
