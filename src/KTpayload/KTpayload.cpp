#include "I2Cdev.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "Wire.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP085.h"

const int MPU_ADDR = 0x68;

// Magnetometer class default I2C address is 0x1E
HMC5883L mag1;
HMC5883L mag2;
float m1x, m1y, m1z;
float m2x, m2y, m2z;

// Accel/Gyro class default I2C address is 0x68 (can be 0x69 if AD0 is high)
MPU6050 accelgyro1;
MPU6050 accelgyro2;
int16_t a1x, a1y, a1z;
int16_t a2x, a2y, a2z;
int16_t g1x, g1y, g1z;
int16_t g2x, g2y, g2z;

// BMP085 sensors
Adafruit_BMP085 bmp1;
Adafruit_BMP085 bmp2;

void TCA9548A(uint8_t i){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << i);            // send byte to select channel i
  Wire.endTransmission();
}

void displaySensorDetails1(int i)
{
  TCA9548A(i);
  accelgyro1.getMotion6(&a1x, &a1y, &a1z, &g1x, &g1y, &g1z);
  mag1.getHeading(&m1x, &m1y, &m1z);
  float temp1 = bmp1.readTemperature();
  float alt1 = bmp1.readAltitude();

  Serial.print(i); Serial.print(",");
  Serial.print(a1x/4096.0); Serial.print(",");
  Serial.print(a1y/4096.0); Serial.print(",");
  Serial.print(a1z/4096.0); Serial.print(",");
  Serial.print(g1x/131); Serial.print(",");
  Serial.print(g1y/131); Serial.print(",");
  Serial.print(g1z/131); Serial.print(",");
  Serial.print(m1x); Serial.print(",");
  Serial.print(m1y); Serial.print(",");
  Serial.print(m1z); Serial.print(",");
  Serial.print(temp1); Serial.print(",");
  Serial.print(alt1); Serial.print(";");
}

void displaySensorDetails2(int i)
{
  TCA9548A(i);
  accelgyro2.getMotion6(&a2x, &a2y, &a2z, &g2x, &g2y, &g2z);
  mag2.getHeading(&m2x, &m2y, &m2z);
  float temp2 = bmp2.readTemperature();
  float alt2 = bmp2.readAltitude();

  Serial.print(i); Serial.print(",");
  Serial.print(a2x/4096.0); Serial.print(",");
  Serial.print(a2y/4096.0); Serial.print(",");
  Serial.print(a2z/4096.0); Serial.print(",");
  Serial.print(g2x/131); Serial.print(",");
  Serial.print(g2y/131); Serial.print(",");
  Serial.print(g2z/131); Serial.print(",");
  Serial.print(m2x); Serial.print(",");
  Serial.print(m2y); Serial.print(",");
  Serial.print(m2z); Serial.print(",");
  Serial.print(temp2); Serial.print(",");
  Serial.println(alt2);
}

void setup()
{
  Serial.begin(921600);
  Wire.begin();
 
  TCA9548A(3);
  accelgyro1.initialize();
  Serial.println(accelgyro1.testConnection() ? "MPU6050 1 connection successful" : "MPU6050 1 connection failed");   
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C);
  Wire.write(0b00010000); // Setting the accel to +/- 8g
  Wire.endTransmission(true); 
  accelgyro1.setI2CBypassEnabled(true); // set bypass mode
  mag1.initialize();
  Serial.println(mag1.testConnection() ? "HMC5883L 1 connection successful" : "HMC5883L 1 connection failed");
  if (!bmp1.begin()) {
    Serial.println("BMP085 1 initialization failed");
  }

  TCA9548A(6);   
  accelgyro2.initialize();
  Serial.println(accelgyro2.testConnection() ? "MPU6050 2 connection successful" : "MPU6050 2 connection failed"); 
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C);
  Wire.write(0b00010000); // Setting the accel to +/- 8g
  Wire.endTransmission(true); 
  accelgyro2.setI2CBypassEnabled(true); // set bypass mode
  mag2.initialize();
  Serial.println(mag2.testConnection() ? "HMC5883L 2 connection successful" : "HMC5883L 2 connection failed");
  if (!bmp2.begin()) {
    Serial.println("BMP085 2 initialization failed");
  }
}

void loop()
{
  static unsigned long ms = 0;
  
  if (millis() - ms > 12) // delay time
  {
    Serial.print(ms); Serial.print(",");      
    displaySensorDetails1(3);
    displaySensorDetails2(6);
       
    ms = millis();
  }
}
