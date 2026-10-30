# doomstick

**doomstick** is a compact, dual-button ESP32-C3 toolkit designed for controlled wireless research and educational experimentation in authorized environments.

## 1. Introduction

doomstick brings a focused set of Wi-Fi and related radio tools onto a tiny ESP32-C3 SuperMini board paired with a 0.96" OLED and two simple buttons.  
It is intended for learning, demonstration, and testing inside labs, classrooms, or other spaces where you have explicit permission to operate.

The interface is deliberately minimal: one button cycles through options, the other selects. Hold the select button to exit any active tool and return to the main menu.

## 2. Hardware

- **MCU**: ESP32-C3 SuperMini  
- **Display**: 0.96-inch SSD1306 OLED (64×128, portrait orientation)  
- **Input**: Two tactile buttons (Move + Select)  
- **Form factor**: Extremely small, breadboard-friendly, USB-C powered  

No extra peripherals (NeoPixels, RFID readers, etc.) are required. Everything needed is the board, the OLED, and the two buttons.

## 3. Features

- **Beacon Spam** – Broadcast custom or selected beacon frames from predefined lists  
- **Wi-Fi Deauth** – Deauthenticate all nearby networks or target a specific access point  
- **SuperSniffer** – Advanced packet inspection and monitoring  
- **Camera Deauther** – Targeted disruption of wireless camera streams  
- **PMKID / EAPOL Capture** – Capture handshakes and PMKID material for offline analysis  
- **Pwnagotchi Spam** – Generate pwnagotchi-style advertising frames  
- **Reaper Extension** – Inter-device coordination and control protocol support  

All tools are presented in a single flat menu for quick access.

## 4. Pinout

| Function       | GPIO |
|----------------|------|
| OLED SDA       | 6    |
| OLED SCL       | 7    |
| Move button    | 3    |
| Select button  | 10   |

Buttons use internal pull-ups (active LOW).  
I²C pins are chosen to avoid the ESP32-C3 strapping pins.

## 5. License

This project is released under the **GNU General Public License v3.0**.

You are free to use, modify, and redistribute the software under the terms of the GPL-3.0.  
See the accompanying `LICENSE` file for the full text.

**Important**: Use only on networks and devices you own or have explicit written authorization to test. Unauthorized use of deauthentication, capture, or disruption tools is illegal in most jurisdictions.

## 6. Inspiration

doomstick draws design inspiration from the **M5Stick** series — small, handheld, button-driven ESP32 devices that proved how much capability can fit into a minimal form factor.  

The goal was to capture that same “tiny but powerful” spirit while focusing exclusively on a curated set of wireless research tools and a pure two-button interface.
