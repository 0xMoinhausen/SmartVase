#include "ultrasonic_sensor.h"

void setupUltrasonicSensor(ULTRASONIC_SENSOR ultrasonicSensor) {
    pinMode(ultrasonicSensor.echo_pin, INPUT);
    pinMode(ultrasonicSensor.trigger_pin, OUTPUT);
}


int getUltrasonicDistanceInCm(ULTRASONIC_SENSOR ultrasonicSensor) {
    return 0.03432 * getUltrasonicDistanceInMicroseconds(ultrasonicSensor);
}

long getUltrasonicDistanceInMicroseconds(ULTRASONIC_SENSOR ultrasonicSensor) {
    delayMicroseconds(5);
    digitalWrite(ultrasonicSensor.trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultrasonicSensor.trigger_pin, LOW);
    return pulseIn(ultrasonicSensor.echo_pin, HIGH, 0);
}