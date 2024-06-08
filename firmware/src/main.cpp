#include <Arduino.h>
#include "ultrasonic_sensor.h"
#include "utils.h"

#define Debug true
// #define debug_print(format, ...) \ 
//   do {if (Debug) Serial.printf(format, __VA_ARGS__);} while (0)

ULTRASONIC_SENSOR ultrasonicSensor = {
  .echo_pin = 12,
  .trigger_pin = 11,
};

void setup() {
  // TODO Baudrate ändern
  Serial.begin(9800);
  setupUltrasonicSensor(ultrasonicSensor);

}

void loop() {
  
}