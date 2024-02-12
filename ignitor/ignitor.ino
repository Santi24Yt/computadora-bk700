#include <SoftwareSerial.h>
#include <wire.h>

#define boton 4

// AT+C003 canal de comunicacion

SoftwareSerial HC12(8, 7); // tx al pin 8, rx al pin 7

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  pinMode(boton, INPUT);
}

void loop() {
  if (digitalRead(boton) == HIGH) {  // se prendió el botón :o
    HC12.println("kaboom");  // mensaje porque sino chilla el hc
    delay(2000);
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// para el otro lado

#include <SoftwareSerial.h>
#include <wire.h>

#define match 3

// AT+C003 // canal de comunicacion

SoftwareSerial HC12(8, 7); // tx al pin 8, rx al pin 7

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  pinMode(match, OUTPUT);
}

void loop() {
   if (HC12.available() > 0) {  // para ver si se le mandó algo (?
    String mensaje = HC12.readString();  // lee el mensaje
  }
   if (mensaje == "kaboom") {
    digitalWrite(match, HIGH);
    delay(1000); // no sé si con eso sea suficionete para prender le motor
    digitalWrite(match, LOW);
  }
}

// yyy no sé si necesitamos màs ematch conetados :o
