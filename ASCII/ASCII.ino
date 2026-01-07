/**
 * @file Ex6_RGB.ino
 * @author
 * @date 2026-01-07
 * @brief Reads RGB values from the Serial Monitor and sets an RGB LED using PWM.
 *
 * @details
 * This sketch expects input in the Serial Monitor as three integers, typically separated
 * by commas, for example: `200,100,40`.
 *
 * It uses:
 * - Serial.parseInt() to extract the three integer values (R, G, B) from the incoming text.
 * - analogWrite() on PWM pins to set LED brightness for each channel (0..255).
 *
 * A small "buffer cleanup" loop removes leftover newline/carriage return characters
 * (\\n, \\r) so they do not interfere with the next read.
 *
 * @hardware
 * - RGB LED (common cathode / common ground):
 *   - Longest leg -> GND
 *   - R, G, B legs -> PWM pins 9, 10, 11 through resistors
 *
 * @note
 * If your RGB LED is common anode (common to +5V), the PWM logic must be inverted:
 * use `255 - value` for each channel. This code assumes common cathode (common ground).
 */

#include <Arduino.h>

/**
 * @name RGB PWM pins
 * @brief PWM pins used to drive the RGB LED channels.
 * @{
 */
const uint8_t pinR = 9;   ///< PWM pin for the red channel
const uint8_t pinG = 10;  ///< PWM pin for the green channel
const uint8_t pinB = 11;  ///< PWM pin for the blue channel
/** @} */

/**
 * @brief Set the RGB LED color using PWM.
 *
 * @details
 * Ensures each channel is clamped to the valid PWM range (0..255), then writes the
 * values using analogWrite(). For a common-cathode RGB LED:
 * - 0   = off
 * - 255 = full brightness
 *
 * @param r Red intensity (0..255)
 * @param g Green intensity (0..255)
 * @param b Blue intensity (0..255)
 */
void setRGB(int r, int g, int b) {            // Takes 3 numbers: red, green and blue
  r = constrain(r, 0, 255);                   // Clamp to valid PWM range
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  analogWrite(pinR, r);                       // Output PWM values to LED channels
  analogWrite(pinG, g);
  analogWrite(pinB, b);
}

/**
 * @brief Arduino setup function.
 *
 * @details
 * Initializes Serial communication and sets RGB pins as OUTPUT.
 * Turns the LED off initially by setting (0,0,0).
 */
void setup() {
  Serial.begin(115200);                       // Start serial communication

  pinMode(pinR, OUTPUT);                      // Configure RGB pins as outputs
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);

  setRGB(0, 0, 0);                            // LED off at startup
}

/**
 * @brief Arduino main loop.
 *
 * @details
 * When serial data is available:
 * 1) Read three integers using Serial.parseInt() -> r, g, b
 * 2) Remove leftover newline/carriage return characters from the buffer
 * 3) Print parsed values for debugging
 * 4) Update the RGB LED using setRGB(r, g, b)
 *
 * @note
 * Serial.parseInt() skips non-numeric characters (like commas), so input formats like
 * `200,100,40` or `200 100 40` both work.
 */
void loop() {
  if (Serial.available() > 0) {               // Check if at least one byte is available
    int r = Serial.parseInt();                // Read first integer
    int g = Serial.parseInt();                // Read second integer
    int b = Serial.parseInt();                // Read third integer

    // Buffer cleanup: remove line ending characters so they don't affect next read
    while (Serial.available() > 0) {
      char c = (char)Serial.peek();           // Look at next char without removing it
      if (c == '\n' || c == '\r') {           // If newline/CR, consume one and stop
        Serial.read();
        break;
      }
      Serial.read();                          // Otherwise discard the character
    }

    // Debug output: show what was parsed
    Serial.print("Parsed -> R=");
    Serial.print(r);
    Serial.print(" G=");
    Serial.print(g);
    Serial.print(" B=");
    Serial.println(b);

    // Apply the color to the RGB LED
    setRGB(r, g, b);
  }
}
