#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
//#include <AsyncJson.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <NonBlockingRtttl.h>
#include <L298N.h>
#include <SPIFFS.h>

#include "ultrasonic_sensor.h"
#include "utils.h"

// Definiert den DHT-Sensortyp
#define DHTTYPE DHT11
// Pin-Zuordnung für verschiedene Sensoren und Aktoren
#define BUZZER_PIN 4
#define WATER_SENSOR_PIN 35
#define SOIL_SENSOR_PIN 34

// Initialisiert den DHT-Sensor
DHT dht(21, DHTTYPE);
// Erstellt einen Webserver auf Port 80
AsyncWebServer server(80);
// Initialisiert die Wasserpumpe mit den entsprechenden Pins
L298N pump(25, 33);
// Initialisiert den Ultraschallsensor mit den entsprechenden Pins
ULTRASONIC_SENSOR ultrasonicSensor = {
  .echo_pin = 18,
  .trigger_pin = 19,
};

// Konstanten für die Wasserhöhe
const int MAX_WATER_HEIGHT = 2600;
const int MIN_WATER_HEIGHT = 2000;

// Einstellbare Werte für die Sensoren und die Bewässerungszeit
int analogSoilStartWateringValue = 3100;
int wateringTimeinSec = 5;
float temperature = 25;
float humidity = 0;
int waterLevel = 4;
int triggerDistance = 42;
int counterSoundPlayed = 0;

// Funktionsdeklarationen
void notFound(AsyncWebServerRequest *request);
void setupWebserver();
float get_water_height();
void playSound();

// WLAN-Zugangsdaten
const char *ssid = "FRITZ!Repeater 2400";
const char *password = "75763178721406690002";
// const char *ssid = "UBBST-WLAN";
// const char *password = "Game2016";

// Melodie für den Buzzer
const char *simpeons = "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";

void setup() {
  // Initialisiert das SPIFFS-Dateisystem
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Initialisiert die serielle Kommunikation
  Serial.begin(115200);
  // Setzt den Buzzer-Pin als Ausgang
  pinMode(BUZZER_PIN, OUTPUT);
  // Initialisiert den Ultraschallsensor
  setupUltrasonicSensor(ultrasonicSensor);
  // Initialisiert den Webserver
  setupWebserver();
  // Startet den DHT-Sensor
  dht.begin();
}

void loop() {
  // Simulierte Temperaturaktualisierung für Debugging-Zwecke
  if (temperature > 100) {
    temperature = 0;
  } 
  temperature += 1;

  // Misst die Distanz mit dem Ultraschallsensor
  float distance = getUltrasonicDistanceInCm(ultrasonicSensor);
  // Spielt einen Ton, wenn die Distanz kleiner als der Triggerabstand ist
  if ((int) distance <= triggerDistance && distance >= 5 && get_water_height() <= 25) {
    playSound();
  }
  
  // Startet oder stoppt die Wasserpumpe basierend auf dem Bodenfeuchtigkeitswert
  if (analogRead(SOIL_SENSOR_PIN) > analogSoilStartWateringValue) {
    pump.forward();
  } else {
    pump.stop();
  }
  // Spielt die Melodie ab
  rtttl::play();
}

// Spielt eine Melodie ab, wenn sie nicht bereits abgespielt wird
void playSound() {
  if (!rtttl::isPlaying()) {
    rtttl::begin(BUZZER_PIN, simpeons);
    counterSoundPlayed ++;
  }
}

// Berechnet die Wasserhöhe basierend auf dem Sensorwert
float get_water_height() {
  int readWaterSensorValue = analogRead(WATER_SENSOR_PIN);
  float delta = readWaterSensorValue - MIN_WATER_HEIGHT;
  float delta2 = MAX_WATER_HEIGHT - MIN_WATER_HEIGHT;
  return delta / delta2 * 100;
}

// Sendet eine 404-Fehlermeldung, wenn eine Seite nicht gefunden wird
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

// Richtet den Webserver ein
void setupWebserver() {
  // Verbindet sich mit dem WLAN
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wartet, bis die Verbindung hergestellt ist
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFI Failed!\n");
  }

  // Gibt die IP-Adresse des Geräts aus
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println(temperature);

  // Definiert die Endpunkte des Webservers
  server.on(
      "/values",
      HTTP_GET,
      [](AsyncWebServerRequest *request) {
        Serial.println(temperature);
          // Erstellt ein JSON-Dokument mit den Sensorwerten
          StaticJsonDocument<300> data;
          data["temperature"] = dht.readTemperature();
          data["waterLevel"] = get_water_height();
          data["humidity"] = dht.readHumidity();
          data["counterSoundPlayed"] =  counterSoundPlayed;
          data["triggerDistance"] = triggerDistance;
          String response;
          serializeJson(data, response);
          // Sendet die Sensorwerte als JSON-Antwort
          request->send(200, "application/json", response);
      }
  );
  server.on(
    "/playSound",
    HTTP_POST,
    [](AsyncWebServerRequest *request) {
      playSound();
      request->send(200, "text/plain", "");
    }
  );

  server.on(
    "/dashboard",
    HTTP_GET,
    [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/dashboard.html");
    }
  );

  // Definiert den 404-Fehlerhandler
  server.onNotFound(notFound);

  // Startet den Webserver
  server.begin();
}