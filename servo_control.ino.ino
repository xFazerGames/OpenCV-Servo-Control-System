#include <Servo.h>
Servo servo1;

String entradaSerial = "";         // String to store input
bool entradaCompleta = false;  // Indicates if the String is complete

int pin = 9;    // PWM connection pin to the servo
int pulsoMinimo = 580;  // Pulse duration in microseconds to turn 0º
int pulsoMaximo = 2500; // Pulse duration in microseconds to turn 180º
int angulo = 0; // Variable to store the desired angle of rotation

void setup()
{
  servo1.attach(pin, pulsoMinimo, pulsoMaximo);
  Serial.begin(9600);
}

void loop()
{
  if (entradaCompleta) {
    if (entradaSerial == "izq1\n") {
      Serial.print("0 degrees\n");
      // Send the desired angle for rotation.
      servo1.write(0);
    }
    else if (entradaSerial == "izq2\n") {
      Serial.print("30 degrees\n");
      // Send the desired angle for rotation.
      servo1.write(30);
    }
    else if (entradaSerial == "izq3\n") {
      Serial.print("60 degrees\n");
      // Send the desired angle for rotation.
      servo1.write(60);
    }
    else if (entradaSerial == "ctr\n") {
      Serial.print("Center 90 degrees\n");
      // Send the desired angle for rotation.
      servo1.write(90);
    }
    else if (entradaSerial == "der3\n") {
      Serial.print("120 degrees\n");
      // Send the desired angle for rotation.
      servo1.write(120);
    }
    else if (entradaSerial == "der2\n") {
      Serial.print("150 degrees\n");
      // Send the desired angle for rotation.
      servo1.write(150);
    }
    else if (entradaSerial == "der1\n") {
      Serial.print("180 degrees\n");
      // Send the desired angle for rotation.
      servo1.write(180);
    }
    else { // Any other received data
      Serial.println("The received data is invalid!!");
    }
    entradaSerial = "";
    entradaCompleta = false;
  }
}

// Function that is triggered when receiving something via
// the serial port, Serial Port Interrupt.
void serialEvent() {
  while (Serial.available()) {
    // Get incoming bytes:
    char inChar = (char)Serial.read();
    // Add to the input String:
    entradaSerial += inChar;
    // To know if the string is complete, it will stop when receiving
    // the newline character ENTER \n
    if (inChar == '\n') {
      entradaCompleta = true;
    }
  }
}

