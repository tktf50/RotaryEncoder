/**
 * @file DualEncoders.cpp
 * @author Steven Dicker
 * @brief Example program demonstrating use of multiple interrupt driven rotary encoders
 * @version 1.0
 * @date 2026-04-11
 * 
 * @copyright Copyright (c) 2026
 * This example software is released under the GNU GPL Version 3 License.
 * https://www.gnu.org/licenses/gpl-3.0.html
 */
#include <Arduino.h>
#include <RotaryEncoder.h>

// Signal pins for the two encoders
#define DT1_PIN 18
#define CLK1_PIN 19
#define DT2_PIN 21
#define CLK2_PIN 22

RotaryEncoder<DT1_PIN, CLK1_PIN> encoder1;
RotaryEncoder<DT2_PIN, CLK2_PIN> encoder2;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  // Initialize the interface to the encoders for use with interrupts.
  encoder1.begin(true);
  encoder2.begin(true);
}

void loop() {
  // Report encoder activity. You should be able to turn both encoders at the
  // same time and see reports from both.
  unsigned char direction = encoder1.getAndResetEncoderDir();
  if (DIR_NONE != direction) {
    Serial.print("Encoder 1 turned ");
    Serial.println(DIR_CW == direction ? "Clockwise"
                                       : "Counterclockwise");
  }
  direction = encoder2.getAndResetEncoderDir();
  if (DIR_NONE != direction) {
    Serial.print("Encoder 2 turned ");
    Serial.println(DIR_CW == direction ? "Clockwise"
                                       : "Counterclockwise");
  }
  delay(10);
}