// Constantes
// El pin en el que está conectado el buzzer (D2 en este caso)
#define buzzer 2
// El pin del sistema de recuperación
#define deploy 3


// Segundos del timer para que se despliegue el paracaidas
#define timer 30


void setup()
{
  // Definir el pin del buzzer como pin de salida
  pinMode(buzzer, OUTPUT);
  // Definir el pin para el mosfet del sistema de recuperación
  pinMode(deploy, OUTPUT);

  // Todo es correcto, sonar el buzzer
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);

  delay(3000);
}


void loop()
{
  for (int i = 0; i < timer; i++)
  {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
  }

  digitalWrite(deploy, HIGH);
  delay(4000);
  digitalWrite(deploy, LOW);

  digitalWrite(buzzer, HIGH);

  while(1);

  digitalWrite(buzzer, LOW);
}

