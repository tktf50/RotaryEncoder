/**
 * @file Polled.cpp
 * @author Steven Dicker
 * @brief Example program demonstrating use of polling to interface with
 *         rotary encoders
 * @version 1.0
 * @date 2026-04-11
 * 
 * @copyright Copyright (c) 2026
 * This example software is released under the GNU GPL Version 3 License.
 * https://www.gnu.org/licenses/gpl-3.0.html
 */
#include <Arduino.h>
#include <RotaryEncoder.h>

#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
#define DT_PIN 21
#define CLK_PIN 22
#else
#define DT_PIN 2
#define CLK_PIN 3
#endif

RotaryEncoder<DT_PIN, CLK_PIN> encoder;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Setup complete");
  // Initialize interface to rotary encoder for polling
  encoder.begin(true, true);
}

void loop() {
  // Poll the encoder to check for turns. Note that polling is more likely (than
  // interrupt based processing) to miss turns if you don't poll frequently enough.
  // If the reported direction is incorrect, swap the connections to the CLK and DT pins.
  unsigned char direction = encoder.process();
  if (DIR_NONE != direction) {
    Serial.print("Encoder turned ");
    Serial.println(DIR_CW == direction ? "Clockwise"
                                       : "Counterclockwise");
  }
  delay(10);
}