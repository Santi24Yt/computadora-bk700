#include <SoftwareSerial.h>
#include <Wire.h>

#define setin 4
#define setout 13

#define boton 10

#define contra 147

// AT+C003 canal de comunicacion

SoftwareSerial HC12(8, 7); // tx al pin 8, rx al pin 7

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);

  pinMode(boton, INPUT);
  pinMode(setin, INPUT);
  pinMode(setout, OUTPUT);

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

    if (digitalRead(boton) == HIGH) {  // se prendió el botón :o
      Serial.println(contra);
      HC12.println(contra);  // mensaje porque sino chilla el hc
      delay(2000);
    }
  }

  
}
