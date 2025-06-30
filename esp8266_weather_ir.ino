#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <time.h>
#include <math.h>  // Für cos/sin bei Sonne

// WLAN-Zugang
const char* ssid = "Your ssid";
const char* password = "Your ssid password";

// OpenWeatherMap API
const char* host = "api.openweathermap.org";
const String apiKey = "6124093a3bb8f8c9ea420496f4e34c12";

// Städte & Zeitzonen
struct Stadt {
  String name;
  String id;
  int gmtOffset;  // in Sekunden
};

Stadt stadtListe[] = {
  {"China", "1816670", 8 * 3600},
  {"Luxemburg", "2960316", 2 * 3600},
  {"Antarktis", "6255152", 0},
  {"USA", "5128581", -4 * 3600}
};

int aktuelleStadt = 0;

// IR
const uint16_t irPin = D5;
IRrecv irrecv(irPin);
decode_results results;

// OLED Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Wetterdaten
String weatherMain = "";
float temperature = 0.0;

// Zeitsteuerung
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 1000;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  u8g2.begin();
  irrecv.enableIRIn();

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 20, "Verbinde WLAN...");
  u8g2.sendBuffer();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  configTime(stadtListe[aktuelleStadt].gmtOffset, 0, "pool.ntp.org", "time.nist.gov");

  u8g2.clearBuffer();
  u8g2.drawStr(0, 20, "WLAN verbunden!");
  u8g2.sendBuffer();
  delay(1000);

  getWeatherData();
  drawDisplay();
}

void loop() {
  // IR Stadtwahl
  if (irrecv.decode(&results)) {
    uint32_t code = results.value;
    int neueStadt = aktuelleStadt;

    if (code == 0xFFA25D) neueStadt = 0;  // Taste 1
    else if (code == 0xFF629D) neueStadt = 1; // Taste 2
    else if (code == 0xFFE21D) neueStadt = 2; // Taste 3
    else if (code == 0xFF22DD) neueStadt = 3; // Taste 4

    if (neueStadt != aktuelleStadt) {
      aktuelleStadt = neueStadt;
      configTime(stadtListe[aktuelleStadt].gmtOffset, 0, "pool.ntp.org", "time.nist.gov");
      getWeatherData();
      drawDisplay();
    }

    irrecv.resume();
  }

  // Zeit jede Sekunde aktualisieren
  if (millis() - lastUpdate >= updateInterval) {
    drawDisplay();
    lastUpdate = millis();
  }
}

void getWeatherData() {
  WiFiClient client;
  String cityID = stadtListe[aktuelleStadt].id;

  if (!client.connect(host, 80)) {
    Serial.println("Verbindung fehlgeschlagen");
    return;
  }

  String url = "/data/2.5/weather?id=" + cityID + "&appid=" + apiKey + "&units=metric&lang=de";
  client.print(String("GET ") + url + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
  }

  String payload = client.readString();
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  weatherMain = doc["weather"][0]["main"].as<String>();
  temperature = doc["main"]["temp"];
}

void drawDisplay() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  // Stadtname
  u8g2.setCursor(0, 12);
  u8g2.print("Land: ");
  u8g2.print(stadtListe[aktuelleStadt].name);

  // Uhrzeit
  u8g2.setCursor(0, 24);
  u8g2.print("Zeit: ");
  printLocalTime();

  // Temperatur
  u8g2.setCursor(0, 36);
  u8g2.print("Temp: ");
  u8g2.print(temperature, 1);
  u8g2.print(" C");

  // Wetterbeschreibung
  u8g2.setCursor(0, 48);
  u8g2.print("Zustand: ");
  u8g2.print(weatherMain);

  // Wetter-Animation
  if (weatherMain == "Clear") {
    u8g2.drawCircle(110, 20, 7);  // Sonne
    for (int i = 0; i < 360; i += 45) {
      int x1 = 110 + cos(i * DEG_TO_RAD) * 10;
      int y1 = 20 + sin(i * DEG_TO_RAD) * 10;
      u8g2.drawLine(110, 20, x1, y1);
    }
  }
  else if (weatherMain == "Rain") {
    u8g2.drawBox(95, 15, 30, 10);  // Wolke
    for (int i = 0; i < 3; i++)
      u8g2.drawLine(100 + i * 8, 26, 100 + i * 8, 35);  // Tropfen
  }
  else if (weatherMain == "Clouds") {
    u8g2.drawRBox(95, 18, 28, 12, 4);  // runde Wolke
    u8g2.drawCircle(100, 18, 6);
    u8g2.drawCircle(115, 20, 5);
  }

  u8g2.sendBuffer();
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    u8g2.print("Fehler");
    return;
  }

  char timeStr[10];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
  u8g2.print(timeStr);
}
