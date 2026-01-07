#include <Arduino.h>

const uint8_t pinR = 9;
const uint8_t pinG = 10;
const uint8_t pinB = 11;

void setRGB(int r, int g, int b) { // Taked 3 numbers red, green and blue
  r = constrain(r, 0, 255);        // set's the max and low so we can apply color to the RHB LED with range 0 - 255
  g = constrain(g, 0, 255);        // ect if r = 300 becomes 255
  b = constrain(b, 0, 255);

  analogWrite(pinR, r);            // analogwrite outputs the values in PWM which means
  analogWrite(pinG, g);            // 0 = off and 255 = fully on, so values inbetween becomes partial brightness
  analogWrite(pinB, b);
}

void setup() {
  Serial.begin(115200);           // Genereal setup for the code in arduino
  pinMode(pinR, OUTPUT);          // Sets the RGB pins as outputs
  pinMode(pinG, OUTPUT);          // Turns off the LEDS off intially with 0,0,0
  pinMode(pinB, OUTPUT);
  setRGB(0, 0, 0);
}

void loop() {                     // Reading the data in the loop
  if (Serial.available() > 0) {  // This checks: is there at least one byte waiting in the serial buff?
    int r = Serial.parseInt();    // if Yes then we start reading
    int g = Serial.parseInt();    // The serial.parseInt() finds and reads the next integer in the incoming text. It ignores non numeric characters like commas and spaces
    int b = Serial.parseInt();    // ect 200,100,40 becomes first int = 200, second Int = 100, Third Int = 40

    
    while (Serial.available() > 0) {    // clean op option that helps with clearing op leftovers, Serial Monitor often sends line endings \n, \r after pressing send
      char c = (char)Serial.peek();     // If they dont get removed they can interfere with the next read, this loop, looks at the next character without removing it, if its newline or carriage return = it reads it once and stops
      if (c == '\n' || c == '\r') { Serial.read(); break; } // othervise it reads and discards the characters until the line ending is reached
      Serial.read();
    }

    Serial.print("Parsed -> R=");
    Serial.print(r);
    Serial.print(" G=");
    Serial.print(g);
    Serial.print(" B=");
    Serial.println(b);

    setRGB(r, g, b);
  }
}
