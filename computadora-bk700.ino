// Constantes
// Número máximo de archivos que puede crear en la sd (máximo 254 creo)
#define MAX_FILES 99
// El pin en el que está conectado el buzzer (D2 en este caso)
#define buzzer 2
// El pin del sistema de recuperación
#define deploy 3

// Librería para comunicación I²C
#include <Wire.h>
// Librería para manejar más fácilmente los sensores
#include <Adafruit_Sensor.h>
// Librería relevante al sensor de presión BMP
#include <Adafruit_BMP085_U.h>
// Librería relevante al sensor de aceleración, rotación y temperatura MPU6050
#include <Adafruit_MPU6050.h>
// Librería para comunicación SPI
#include <SPI.h>
// Librería para manejar la tarjeta SD
#include <SD.h>

// Definir el tipo de sensor BMP
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
// Definir el sensor MPU6050
Adafruit_MPU6050 mpu = Adafruit_MPU6050();
//para el tiempo
unsigned long tiempo=0;

// Definir el archivo en el que se van a guardar los datos
File archivo;

// Definir firma de la función que detiene el programa si hay algún error fatal
void fatal(String err);

void setup() {
  // Inicializar el serial
  Serial.begin(9600);

  // Definir el pin del buzzer como pin de salida
  pinMode(buzzer, OUTPUT);
  // Definir el pin para el mosfet del sistema de recuperación
  pinMode(deploy, OUTPUT);

  // Checar si se puede inicializar la sd
  if (!SD.begin())
  {
    fatal("sd no inicia");
  }

  // Nombre default del archivo de datos
  char name[15];
  // Revisar que número de archivo de vuelo no existe para poder crear uno en ese caso
  for (unsigned char i = 0; i <= MAX_FILES; i++)
  {
    sprintf(name, "vuelo%d.txt", i);
    if (!SD.exists(name))
    {
      break;
    }
    // Si ya existen todos usar default.txt
    if (i == MAX_FILES)
    {
      sprintf(name, "default.txt");
    }
  }

  // Abrir el archivo
  archivo = SD.open(name, FILE_WRITE);
  // Checar si sí abrió el archivo
  if (!archivo)
  {
    fatal("archivo no abre");
  }

  // Escribir en el archivo el nombre de las columnas de datos
  archivo.println("tiempo,presion,altura,aceleracion x,aceleracion y,aceleracion z,rotacion x,rotacion y,rotacion z,temperatura");
  // Guardar los datos sin cerrar el archivo
  archivo.flush();

  // Checar si se detecta el BMP
  if (!bmp.begin())
  {
    fatal("presion no detectada");
  }

  // Checar si se detecta el MPU
  if (!mpu.begin())
  {
    fatal("aceleracion no detectada");
  }

  // Fijar el rango del MPU a +-4g
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);

  // Todo es correcto, sonar el buzzer
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);

}

// Definir el número de iteraciones que tienen que pasar para guardar los datos en el archivo ya que guardarlos en cada loop no es eficiente
#define FLUSH_ITER 20
// Número de iteraciones
unsigned char iter = 0;

// presion base
float presionbase = -1000;
// Alocar la altura
float altura;
// Alocar el apogeo
float apogeo;

// Alocar la aceleración vertical
float accelz;

// Fases de vuelo, fase 0 es antes del despegue
unsigned char fase = 0;

void loop() {

  // Los eventos del sensor
  sensors_event_t p, a, g, temp;
  // Obtener el evento de presión
  bmp.getEvent(&p);
  // Obtener los eventos de aceleración, giroscopio y temperatura
  mpu.getEvent(&a, &g, &temp); 

  // Determinar la altura
  // Si aún no se establece la presión (altitud) base entonces establecerla
  if (presionbase == -1000)
  {
    presionbase = p.pressure;
  }
  // Calcular la altura en base a la altitud actual y la presion (altitud) base
  altura = bmp.pressureToAltitude(presionbase, p.pressure); 
  // Checar si la altura actual es mayor a la máxima
  if (altura > apogeo)
  {
    apogeo = altura;
  }

  accelz = a.acceleration.z;
  // Cuando la aceleración vertical sea mayor a 2m/s^2 posiblemente ya despegó (ya que usualmente es -10 m/s^2 por la gravedad)
  if (fase == 0 && accelz >= 2)
  {
    // Fase 1 es que ya despegó
    fase = 1;
    Serial.println("CAMBIO A FASE 1");
    //intento de que aquí marque 0 y no desde que se prende, no hay mucha fe
    tiempo = millis();
  }

  // Si ya despegó y la altura es mayor a 20 y ya pasó el apogeo entonces pasar a la fase 2 y desplegar el sistema de recuperación
  if (fase == 1 && altura >= 22 && altura <= apogeo-5)
  {
    fase = 2;
    digitalWrite(deploy, HIGH);
    delay(1000);
    digitalWrite(deploy, LOW);
    Serial.println("CAMBIO A FASE 2");
  }


  if (fase == 2)
  {
    digitalWrite(buzzer, HIGH);
    // Señales de estado
  }

  //escribir el tiempo 
  archivo.print(tiempo,4);
  archivo.print(",");
  
  if (p.pressure)
  {
    // Escribir la presión
    // Serial.print("Pressure: ");
    // Serial.print(p.pressure);
    archivo.print(p.pressure);
    archivo.print(",");
    // Serial.println(" hPa");
    // Escribir la altura
    // Serial.print("Altura: ");
    // Serial.println(altura);
    archivo.print(altura);
    archivo.print(",");
  }

  // Escribir datos del MPU
  // Serial.print("Acceleration X: ");
  // Serial.print(a.acceleration.x);
  archivo.print(a.acceleration.x);
  archivo.print(",");
  // Serial.print(", Y: ");
  // Serial.print(a.acceleration.y);
  archivo.print(a.acceleration.y);
  archivo.print(",");
  // Serial.print(", Z: ");
  // Serial.print(a.acceleration.z);
  archivo.print(a.acceleration.z);
  archivo.print(",");
  // Serial.println(" m/s^2");

  // Serial.print("Rotation X: ");
  // Serial.print(g.gyro.x);
  archivo.print(g.gyro.x);
  archivo.print(",");
  // Serial.print(", Y: ");
  // Serial.print(g.gyro.y);
  archivo.print(g.gyro.y);
  archivo.print(",");
  // Serial.print(", Z: ");
  // Serial.print(g.gyro.z);
  archivo.print(g.gyro.z);
  archivo.print(",");
  // Serial.println(" rad/s");

  // Serial.print("Temperature: ");
  // Serial.print(temp.temperature);
  archivo.print(temp.temperature);
  archivo.println();
  // Serial.println(" degC");

  // Aumentar el número de iteraciones
  iter++;

  // Guardar el archivo si ya pasó el número de iteraciones deseado, en caso de que aún no se haya guardado automáticamente
  if(iter >= FLUSH_ITER)
  {
    archivo.flush();
    iter = 0;
  }

  // Delay entre loops potencialmente inecesario
  // delay(500);
}

// Funcion para errores fatales, no permite que el programa continue
void fatal(String err)
{
  Serial.println(err);
  while(1);
}
