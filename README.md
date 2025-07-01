# 🌍 Arduino Wetterstation mit OLED, Weltzeit & Wetteranimation

Dieses Projekt zeigt Temperatur, Wetterzustand und lokale Uhrzeit für vier verschiedene Städte auf einem OLED-Display an. Mit einem Knopf kannst du zwischen den Städten umschalten. Die Wetterdaten werden live von OpenWeatherMap geladen, und die Zeit wird über NTP synchronisiert.

---
## Material
- ESP8266
- IR Sonsor
- IR Fernbedinung
- QIQIAZI 0,96 Zoll OLED Display I2C Modul
- 2,4GHz Wifi
## 🔧 Features

- 📡 Live-Wetterdaten von OpenWeatherMap
- 🕒 Lokale Zeit für jede Stadt (über NTP & GMT Offset)
- 🌦️ Anzeige von Temperatur & Wetterbeschreibung
- 🖼️ Einfache Wetteranimationen (Sonne, Wolken, Regen)
- 🧭 Wechsel zwischen vier Städten per Knopfdruck
- 💾 Kompatibel mit ESP8266 (z. B. NodeMCU, Wemos D1 mini)

---

## ⚙️ Hardware

- ESP8266 Board (NodeMCU oder Wemos D1 Mini)
- 0.96" I2C OLED Display (SSD1306, 128x64)
- Taster zwischen D5 und GND
- USB-Kabel & WLAN-Verbindung

---

## 🔩 Schaltplan

| Komponente     | ESP8266 Pin |
|----------------|-------------|
| OLED SDA       | D2          |
| OLED SCL       | D1          |
| Taster         | D5 ↔ GND     |

OLED-Adresse ist `0x3C` (Standard für SSD1306 I2C).

---

## 🧰 Benötigte Libraries

Installiere diese Bibliotheken über den Bibliotheksmanager in der Arduino IDE:

- `ESP8266WiFi`
- `ESP8266HTTPClient`
- `ArduinoJson`
- `Adafruit GFX`
- `Adafruit SSD1306`

---

## 🌐 Wetter-API (OpenWeatherMap)

1. Erstelle einen kostenlosen Account auf https://openweathermap.org/
2. Hole dir deinen API-Schlüssel (API Key)
3. Trage ihn im Code ein:

```cpp
const String apiKey = "HIER_DEIN_API_KEY";
