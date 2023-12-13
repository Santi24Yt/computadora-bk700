#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_MPU6050.h>
#include <SD.h> //creo que esa es la libreria

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
Adafruit_MPU6050 mpu = Adafruit_MPU6050();
Adafruit_Sensor sensor = Adafruit_Sensor();
SD sd = SD();

//insertar pines de la sd

void setup() {
  Serial.begin(9600);

  if (!bmp.begin())
  {
    //funcion del buzzer para decir que no jalò
    digitalWrite(Sensor,HIGH); //lo prende
    delay(1000); // va a hacer que dure un sec el sonido
    digitalWrite(Sensor,LOW); //lo apaga
  }

  if (!mpu.begin())
  {
    digitalWrite(Sensor,HIGH); //lo prende
    delay(1000); // va a hacer que dure un sec el sonido
    digitalWrite(Sensor,LOW); //lo apaga
  }else{
    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  }

  if (!sd.begin())
  {
    digitalWrite(Sensor,HIGH); //lo prende
    delay(1000); // va a hacer que dure un sec el sonido
    digitalWrite(Sensor,LOW); //lo apaga
  }
}

void loop() {

  sensors_event_t p, a, g, temp;
  bmp.getEvent(&p);
  mpu.getEvent(&a, &g, &temp);

  if (p.pressure)
  {
    Serial.print("Pressure: ");
    Serial.print(p.pressure);
    Serial.println(" hPa");
  }

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("----------");
  delay(500);
}
