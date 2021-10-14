// Libaries 
#include <WiFi.h>
#include <MPU9250_asukiaaa.h>

// de
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;

void setup() {

}

void loop() {

}
