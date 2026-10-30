/*
    doomstick - controlled-environment subset of doomBOX
    Original: doomBOX by Richard Gladson
    https://github.com/RichardGladson/doomBOX
    Copyright (c) 2026 Richard Gladson

    Licensed under the GNU General Public License v3.0
    https://www.gnu.org/licenses/gpl-3.0.html

    SPDX-License-Identifier: GPL-3.0-only

    Hardware: ESP32-C3 SuperMini
              0.96" SSD1306 OLED (128x64 software / portrait mount)
              2 buttons: MOVE (GPIO3), SELECT (GPIO10)
              I2C: SDA=GPIO6, SCL=GPIO7
*/
#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <esp_wifi.h>

#include "../include/pindefs.h"
#include "../include/sleep_manager.h"
#include "../include/display_mirror.h"
#include "../include/setting.h"
#include "../include/radio_manager.h"

#include "../include/beacon_spam.h"
#include "../include/deauth.h"
#include "../include/handshake_capture.h"
#include "../include/camera_deauther.h"
#include "../include/pwnagotchi_spam.h"
#include "../include/superscanner.h"
#include "../include/reaper_ui.h"

// Keep original type so existing modules compile unchanged
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

bool dangerousActionsEnabled = true;

// ---- sleep / activity stubs (required by modules) ----
unsigned long lastActivity = 0;

void updateLastActivity() {
  lastActivity = millis();
}

void updateSleepTimeout(unsigned long) {}
void wakeDisplay() {}
void checkIdle() {}

bool anyButtonPressed() {
  return digitalRead(BUTTON_PIN_MOVE) == LOW ||
         digitalRead(BUTTON_PIN_SELECT) == LOW;
}

// ---- simple 2-button menu ----
struct AppEntry {
  const char* name;
  void (*setup)();
  void (*loop)();
  void (*cleanup)();
};

void noCleanup() {}
void cleanupWiFi() {
  esp_wifi_stop();
  esp_wifi_deinit();
}

AppEntry apps[] = {
  { "Beacon Spam",      beaconSpamSetup,         beaconSpamLoop,         cleanupWiFi },
  { "WiFi Deauth",      deauthSetup,             deauthLoop,             cleanupWiFi },
  { "SuperSniffer",     superscannerSetup,       superscannerLoop,       superscannerCleanup },
  { "Camera Deauther",  cameraDeautherSetup,     cameraDeautherLoop,     cameraDeautherCleanup },
  { "PMKID/EAPOL Cap",  handshakeCaptureSetup,   handshakeCaptureLoop,   cleanupWiFi },
  { "Pwnagotchi Spam",  pwnagotchiSpamSetup,     pwnagotchiSpamLoop,     cleanupWiFi },
  { "Reaper",           reaperUiSetup,           reaperUiLoop,           reaperUiCleanup },
};
constexpr int APP_COUNT = sizeof(apps) / sizeof(apps[0]);

int selected = 0;
bool inApp = false;

void drawMenu() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "doomstick");
  u8g2.drawHLine(0, 12, 128);

  // Show 4 items around selection
  int start = selected - 1;
  if (start < 0) start = 0;
  if (start > APP_COUNT - 4) start = max(0, APP_COUNT - 4);

  for (int i = 0; i < 4 && (start + i) < APP_COUNT; i++) {
    int idx = start + i;
    int y = 24 + i * 12;
    if (idx == selected) {
      u8g2.drawBox(0, y - 9, 128, 11);
      u8g2.setDrawColor(0);
      u8g2.drawStr(2, y, apps[idx].name);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(2, y, apps[idx].name);
    }
  }
  u8g2.setFont(u8g2_font_5x7_tf);
  u8g2.drawStr(0, 63, "MOVE=next  SEL=run");
  u8g2.sendBuffer();
  displayMirrorSend(u8g2);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(BUTTON_PIN_MOVE, INPUT_PULLUP);
  pinMode(BUTTON_PIN_SELECT, INPUT_PULLUP);

  Wire.begin(OLED_SDA, OLED_SCL);
  u8g2.begin();
  u8g2.setContrast(200);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(20, 32, "doomstick");
  u8g2.sendBuffer();
  delay(800);

  displayMirrorSetup();
  updateLastActivity();
  drawMenu();
}

void loop() {
  static unsigned long lastMove = 0;
  static unsigned long lastSel  = 0;
  const unsigned long debounce = 250;

  bool moveNow = digitalRead(BUTTON_PIN_MOVE) == LOW;
  bool selNow  = digitalRead(BUTTON_PIN_SELECT) == LOW;

  if (!inApp) {
    if (moveNow && (millis() - lastMove > debounce)) {
      lastMove = millis();
      selected = (selected + 1) % APP_COUNT;
      updateLastActivity();
      drawMenu();
      while (digitalRead(BUTTON_PIN_MOVE) == LOW) delay(10);
    }

    if (selNow && (millis() - lastSel > debounce)) {
      lastSel = millis();
      updateLastActivity();
      while (digitalRead(BUTTON_PIN_SELECT) == LOW) delay(10);

      // Launch app
      inApp = true;
      if (apps[selected].setup) apps[selected].setup();

      // App owns the loop until user exits via its own back logic
      // or we detect a long SELECT hold as emergency exit
      unsigned long holdStart = 0;
      while (inApp) {
        if (apps[selected].loop) apps[selected].loop();

        // Emergency exit: hold SELECT 1.5 s
        if (digitalRead(BUTTON_PIN_SELECT) == LOW) {
          if (holdStart == 0) holdStart = millis();
          else if (millis() - holdStart > 1500) {
            if (apps[selected].cleanup) apps[selected].cleanup();
            inApp = false;
            while (digitalRead(BUTTON_PIN_SELECT) == LOW) delay(10);
            drawMenu();
            break;
          }
        } else {
          holdStart = 0;
        }
        delay(1);
      }
    }
  }
}
