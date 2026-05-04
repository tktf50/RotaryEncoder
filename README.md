# Rotary Encoder Library

This is a variation of Ben Buxton's **Rotary** library with these modifications:

1. Supports interrupt driven operation.
1. Supports multiple rotary encoders when using interrupts.
1. Enabling of pullups is implemented as a method argument rather than
   a compile-time definition.
1. Class has been renamed from Rotary to RotaryEncoder.

### Example Usage
```cpp
#include <Arduino.h>
#include <RotaryEncoder.h>

// Declare an encoder interface instance.
RotaryEncoder<2, 3> encoder;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  // Initialize rotary encoder interface for interrupt based processing
  encoder.begin(true);
}

void loop() {
  // Handle encoder activity.
  unsigned char direction = encoder.getAndResetEncoderDir();
  if (DIR_CW == direction) {
    Serial.println("Encoder turned clockwise");
  }
  else if (DIR_CCW == direction) {
    Serial.println("Encoder turned counterclockwise");
  }
}
```
