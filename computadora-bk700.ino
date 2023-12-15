#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_MPU6050.h>
#include <SD.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
Adafruit_MPU6050 mpu = Adafruit_MPU6050();

SD sd = SD();
File datos;

//char dataStr[100] = "";
//char buffer[7];

void setup() {
  Serial.begin(9600);

  //if (!SD.begin(chipSelect))
  //lo del bee
  
  while (!mpu6050.begin() && !bmp180.begin()) 
  {
    digitalWrite(Sensor,HIGH); //lo prende
    delay(2000); // va a hacer que dure un sec el sonido
    digitalWrite(Sensor,LOW); //lo apaga
  }

  //ahorita vemos
    digitalWrite(Sensor,HIGH); //lo prende
    delay(1000); // va a hacer que dure un sec el sonido
    digitalWrite(Sensor,LOW); //lo apaga
    delay(500);
    digitalWrite(Sensor,HIGH); //lo prende
    delay(1000); // va a hacer que dure un sec el sonido
    digitalWrite(Sensor,LOW); //lo apaga


  datos = SD.open("vuelo.txt", FILE_WRITE);  
   if (datos) // si se abriò correctamete
  {
    datos.println("presion, aceleracion, rotacion y temperatura: ");
    datos.close();
  }else 
    Serial.println("Error abriendo vuelo.txt");
  //Serial.println("l para leer lo que està dentro de vuelo"); (seria para que despliegue todo, pero todavia estoy vienod como conectarlo)
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
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
  //eso fue datos sensores, ahora paracaidas

  float altura =  2020 + (temp.temperature / 6.50) * (((p.pressure / 101200)^((9.8 * 0.029) / (8.314 *6.50)))-1)

  //hacer que si altura > apogeo, entonces apogeo = altura
  
  if (altura > 20 && altura <= apogeo)
  {
 //señal para detonar el e-match aaaaaaaaaaaaaa
  }
}
