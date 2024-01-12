// Constantes
// Número máximo de archivos que puede crear en la sd (máximo 254 creo)
#define MAX_FILES 99
// El pin en el que está conectado el buzzer (D2 en este caso)
#define buzzer 2
// El pin del sistema de recuperación
#define deploy 3


// Cuando la aceleración vertical sea mayor a 2m/s^2 posiblemente ya despegó (ya que usualmente es -10 m/s^2 por la graveda#define ACCZ_MIN_DESPEGUE 2
#define ACCZ_MIN_DESP 2
// Altura mínima para la activacion del sistema de recuperación
#define ALT_MIN_REC 22
// Offset de la altura para evitar que se dispare el sistema de recuperación antes de que llegue verdaderamente al apogeo
// es decir cuantos metros tiene que bajar después del apogeo para que se despliegue
#define ALT_OFF 5


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

// Definir el archivo en el que se van a guardar los datos
File archivo;

// Definir firma de la función que detiene el programa si hay algún error fatal
void fatal(char err);

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
    fatal('a');
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
    fatal('b');
  }

  // Escribir en el archivo el nombre de las columnas de datos
  archivo.println("tiempo,presion,altura,aceleracion x,aceleracion y,aceleracion z,rotacion x,rotacion y,rotacion z,temperatura");
  // Guardar los datos sin cerrar el archivo
  archivo.flush();

  // Checar si se detecta el BMP
  if (!bmp.begin())
  {
    fatal('c');
  }

  // Checar si se detecta el MPU
  if (!mpu.begin())
  {
    fatal('d');
  }

  // Fijar el rango del MPU a +-4g
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);

  // Todo es correcto, sonar el buzzer
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
}


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

  // TODO: Usar switch posiblemente mejorar velocidad

  accelz = a.acceleration.z;
  // Cuando la aceleración vertical sea mayor a 2m/s^2 posiblemente ya despegó (ya que usualmente es -10 m/s^2 por la gravedad)
  if (fase == 0 && accelz >= ACCZ_MIN_DESP)
  {
    // Fase 1 es que ya despegó
    fase = 1;
    // Serial.println("CAMBIO A FASE 1");
  }

  // Si ya despegó y la altura es mayor a 20 y ya pasó el apogeo entonces pasar a la fase 2 y desplegar el sistema de recuperación
  if (fase == 1 && altura >= ALT_MIN_REC && altura <= apogeo - ALT_OFF)
  {
    fase = 2;
    digitalWrite(deploy, HIGH);
    // Se va a perder alrededor de 1s de datos en este punto
    delay(1000);
    digitalWrite(deploy, LOW);
    // Serial.println("CAMBIO A FASE 2");
  }


  // La fase 2 es la caida
  if (fase == 2)
  {
    digitalWrite(buzzer, HIGH);
  }

  // Si está en caida y llega a la altura mínima entonces pasar a las señales de estado y dejar de recolectar datos (fase 3)
  if (fase == 2 && altura <= ALT_MIN_REC)
  {
    fase = 3;
    digitalWrite(buzzer, LOW);
    delay(3000);
  }

  // Detener la recolección de datos y realizar las señales de estado hasta ser apagada
  if (fase == 3)
  {
    archivo.close();
    // Señales de estado
    while (1)
    {
      // Obtener el apogeo y convertirlo a string para poder leer sus caracteres en orden
      char s[8];
      sprintf(s, "%d", (int)altura);
      // Para cada caracter (digito) antes del final dar la señal
      for (char i = 0; i < 8; i++)
      {
        if (s[i] == '\n')
        {
          break;
        }
        char n = s[i] - '0';
        // El digito se identifica con los pulsos cada 100ms
        for (char j = 0; j < n; j++)
        {
          delay(100);
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
        }
        // Cuando acaba el dígito se espera 500ms para poder identificar la separación de digitos
        delay(500);
      }
      // Espera 2s para poder repetir denuevo la señal
      delay(2000);
    }
  }

  //escribir el tiempo 
  archivo.print(millis());
  archivo.print(',');
  
  // Escribir la presión
  // Serial.print("Pressure: ");
  // Serial.print(p.pressure);
  archivo.print(p.pressure);
  archivo.print(',');
  // Serial.println(" hPa");
  // Escribir la altura
  // Serial.print("Altura: ");
  // Serial.println(altura);
  archivo.print(altura);
  archivo.print(',');

  // Escribir datos del MPU
  // Serial.print("Acceleration X: ");
  // Serial.print(a.acceleration.x);
  archivo.print(a.acceleration.x);
  archivo.print(',');
  // Serial.print(", Y: ");
  // Serial.print(a.acceleration.y);
  archivo.print(a.acceleration.y);
  archivo.print(',');
  // Serial.print(", Z: ");
  // Serial.print(a.acceleration.z);
  archivo.print(accelz);
  archivo.print(',');
  // Serial.println(" m/s^2");

  // Serial.print("Rotation X: ");
  // Serial.print(g.gyro.x);
  archivo.print(g.gyro.x);
  archivo.print(',');
  // Serial.print(", Y: ");
  // Serial.print(g.gyro.y);
  archivo.print(g.gyro.y);
  archivo.print(',');
  // Serial.print(", Z: ");
  // Serial.print(g.gyro.z);
  archivo.print(g.gyro.z);
  archivo.print(',');
  // Serial.println(" rad/s");

  // Serial.print("Temperature: ");
  // Serial.print(temp.temperature);
  archivo.print(temp.temperature);
  archivo.println();
  // Serial.println(" degC");

  // Delay entre loops potencialmente inecesario
  // delay(500);
}

// Funcion para errores fatales, no permite que el programa continue
void fatal(char err)
{
  Serial.println(err);
  while(1);
}
