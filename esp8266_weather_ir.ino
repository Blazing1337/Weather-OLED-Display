#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

// ---------- WLAN ----------
const char* ssid = "Jason";
const char* password = "Naruto1337";

// ---------- Display ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- Button ----------
#define BUTTON_PIN D5

// ---------- API ----------
const String apiKey = "6124093a3bb8f8c9ea420496f4e34c12";

// ---------- Städte ----------
struct Stadt {
  String name;
  float lat;
  float lon;
  int gmtOffset; // in Sekunden
};

Stadt stadtListe[] = {
  {"Luxemburg", 49.6117, 6.1319, 2 * 3600},
  {"New York", 40.7128, -74.0060, -4 * 3600},
  {"Peking", 39.9042, 116.4074, 8 * 3600},
  {"Antarktis", -75.2500, -0.0714, 0}
};

int aktuelleStadt = 0;
unsigned long letzteAktualisierung = 0;
String wetter = "";
float temperatur = 0;

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Fehler");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Verbinde WLAN...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WLAN verbunden!");
  display.display();
  delay(1000);

  configTime(stadtListe[aktuelleStadt].gmtOffset, 0, "pool.ntp.org");
  aktualisiereWetter();
}

// ---------- Loop ----------
void loop() {
  static unsigned long letzteSekunde = 0;

  // Zeit anzeigen
  if (millis() - letzteSekunde >= 1000) {
    letzteSekunde = millis();
    zeigeDisplay();
  }

  // Taster gedrückt?
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(300); // Entprellen
    aktuelleStadt = (aktuelleStadt + 1) % (sizeof(stadtListe) / sizeof(Stadt));
    configTime(stadtListe[aktuelleStadt].gmtOffset, 0, "pool.ntp.org");
    aktualisiereWetter();
  }
}

// ---------- Wetter holen ----------
void aktualisiereWetter() {
  WiFiClient client;
  HTTPClient http;

  Stadt stadt = stadtListe[aktuelleStadt];
  String url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
               String(stadt.lat, 4) + "&lon=" + String(stadt.lon, 4) +
               "&appid=" + apiKey + "&units=metric";

  http.begin(client, url);
  int code = http.GET();
  if (code == HTTP_CODE_OK) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    if (!error) {
      wetter = doc["weather"][0]["main"].as<String>();
      temperatur = doc["main"]["temp"];
    }
  }
  http.end();
}

// ---------- Displayanzeige ----------
void zeigeDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Ort: ");
  display.println(stadtListe[aktuelleStadt].name);

  display.print("Temp: ");
  display.print(temperatur);
  display.println(" C");

  display.print("Wetter: ");
  display.println(wetter);

  display.print("Zeit: ");
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    char buffer[10];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
    display.println(buffer);
  } else {
    display.println("Fehler");
  }

  zeichneWetterIcon(wetter);
  display.display();
}

// ---------- Einfaches Wetter-Icon ----------
void zeichneWetterIcon(String zustand) {
  if (zustand == "Clear") {
    display.fillCircle(110, 10, 6, WHITE);
  } else if (zustand == "Clouds") {
    display.fillRect(104, 5, 20, 10, WHITE);
  } else if (zustand == "Rain") {
    for (int i = 0; i < 3; i++) {
      display.drawLine(105 + i * 5, 20, 105 + i * 5, 25, WHITE);
    }
  } else {
    display.setCursor(100, 10);
    display.print("?");
  }
}
