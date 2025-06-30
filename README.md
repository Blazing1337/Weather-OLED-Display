# 🌤️ ESP8266 IR-gesteuertes Wetterdisplay

Ein Projekt für den ESP8266 (z. B. NodeMCU), das Wetterdaten von OpenWeatherMap abruft und sie auf einem OLED-Display anzeigt. Die Stadt kann per IR-Fernbedienung gewechselt werden, die Uhrzeit wird automatisch per NTP aktualisiert.

## 🔧 Funktionen

- Live-Wetterdaten (Temperatur & Zustand)
- Uhrzeit (aktualisiert jede Sekunde per NTP)
- Stadtwechsel per Infrarot-Fernbedienung (Tasten 1–4)
- Wetter-Icons für Sonne, Regen, Wolken

## 🧾 Komponenten

- ESP8266 (z. B. NodeMCU)
- OLED-Display (I2C, 128x64, SSD1306)
- IR-Empfänger (z. B. VS1838B)
- IR-Fernbedienung

## ⬇️ Benötigte Bibliotheken

Installiere diese Bibliotheken über den **Arduino Library Manager**:

- `ESP8266WiFi`
- `WiFiClientSecure`
- `IRremoteESP8266`
- `ArduinoJson`
- `U8g2`
- `time.h` (enthalten im ESP8266 Core)

## 🔑 OpenWeatherMap API

Du brauchst einen API-Key von [https://openweathermap.org/](https://openweathermap.org/)  
Ersetze die Zeile im Code:

```cpp
const String apiKey = "DEIN_API_KEY_HIER";
