# 🌤️ ESP8266 IR-Wetteranzeige mit OLED

Ein Projekt für den ESP8266 (z. B. NodeMCU), das Wetterdaten von OpenWeatherMap anzeigt. Die Stadt kann per IR-Fernbedienung gewechselt werden. Die Zeit wird per NTP jede Sekunde aktualisiert.

## 🔧 Funktionen

- Live-Wetterdaten mit Wetterzustand und Temperatur
- Uhrzeit (per NTP, alle Sekunden aktualisiert)
- IR-Fernbedienung für Stadtwahl (Taste 1–4)
- Wetter-Icons (Sonne, Regen, Wolken)
- OLED 128x64 Display (SSD1306, I2C)

## 🧰 Benötigte Hardware

- ESP8266 (z. B. NodeMCU)
- IR-Empfänger (z. B. VS1838B)
- Fernbedienung (NEC-kompatibel)
- OLED 128x64 (SSD1306, I2C)

## 🔌 Pinbelegung (NodeMCU)

| Modul           | ESP8266 Pin |
|----------------|--------------|
| OLED SCL       | D1           |
| OLED SDA       | D2           |
| IR Signal      | D5           |
| GND / VCC      | G / 3V3      |

## 🧾 Bibliotheken (Arduino IDE)

- ESP8266WiFi (im ESP8266 Board Package enthalten)
- WiFiClientSecure
- IRremoteESP8266
- ArduinoJson
- U8g2
- time.h (eingebaut)

## 📲 IR-Fernbedienung

| Taste | Stadt       |
|-------|-------------|
| 1     | China       |
| 2     | Luxemburg   |
| 3     | Antarktis   |
| 4     | USA         |

## 🌐 OpenWeatherMap API

- Registriere dich bei https://openweathermap.org
- Ersetze `apiKey` im Code durch deinen Schlüssel

```cpp
const String apiKey = "DEIN_API_KEY";
