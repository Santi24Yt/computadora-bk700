#include <SoftwareSerial.h>
#include <Wire.h>

#define setin 4
#define setout 13

#define match 11

#define contra 147

// AT+C003 // canal de comunicacion

SoftwareSerial HC12(8, 7); // tx al pin 8, rx al pin 7

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);

  pinMode(setin, INPUT);
  pinMode(setout, OUTPUT);
  pinMode(match, OUTPUT);

  digitalWrite(setout, HIGH);
}

void loop() {
  if (digitalRead(setin) == HIGH)
  {
    digitalWrite(setout, LOW);

    while (HC12.available())
    {
      Serial.write(HC12.read());
    }

    while (Serial.available())
    {
      HC12.write(Serial.read());
    }
  } else {
    digitalWrite(setout, HIGH);

     if (HC12.available()) {  // para ver si se le mandó algo (?
       if (HC12.parseInt() == contra) {
        Serial.println("1");
         digitalWrite(match, HIGH);
         delay(1000); // no sé si con eso sea suficionete para prender le motor
         digitalWrite(match, LOW);
       }
    }
  }
}
