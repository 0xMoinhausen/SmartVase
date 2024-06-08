#ifndef ULTRASONIC_SENOR_H
#define ULTRASONIC_SENOR_H

#include <Arduino.h>

/**
 * @brief Structure representing an ultrasonic sensor with its associated pins.
 */
typedef const struct UltrasonicSensor {
    u_int8_t echo_pin;
    u_int8_t trigger_pin;
} ULTRASONIC_SENSOR;

/**
 * @brief Sets up the ultrasonic sensor by configuring the echo and trigger pins.
 *
 * @param ultrasonicSensor Pointer to a constant UltrasonicSensor structure containing the pin configuration.
 */
void setupUltrasonicSensor(ULTRASONIC_SENSOR ultrasonicSensor);

/**
 * @brief Measures the distance using the ultrasonic sensor and returns it in centimeters.
 *
 * @param ultrasonicSensor Pointer to an UltrasonicSensor structure containing the pin configuration.
 * @return Distance measured in centimeters.
 */
int getUltrasonicDistanceInCm(ULTRASONIC_SENSOR ultrasonicSensor);

/**
 * @brief Measures the time taken for the ultrasonic signal to bounce back and returns it in microseconds.
 *
 * @param ultrasonicSensor Pointer to a constant UltrasonicSensor structure containing the pin configuration.
 * @return Time measured in microseconds.
 */
long getUltrasonicDistanceInMicroseconds(ULTRASONIC_SENSOR ultrasonicSensor);

#endif