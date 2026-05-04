#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <Arduino.h>

#if !defined(ARDUINO_ARCH_ESP8266) && !defined(ARDUINO_ARCH_ESP32)
#define IRAM_ATTR
#endif

// Enable this to emit codes twice per step (or define it in your platformio.ini)
//#define HALF_STEP

// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

#define R_START 0x0

#ifdef HALF_STEP
extern const unsigned char ttable[6][4];
#else
extern const unsigned char ttable[7][4];
#endif

/**
 * @brief Rotary encoder interface. Provides indication of encoder rotation
 *  and direction of rotation.
 * 
 * Based on the original work of Ben Buxton (Copyright 2011) and released under
 * the GNU GPL Version 3.
 * 
 * @tparam DtPin GPIO connected to the encoder DT pin.
 * @tparam ClkPin GPIO connected to the encoder CLK pin.
 */
template<unsigned char DtPin, unsigned char ClkPin> class RotaryEncoder
{
  public:
    /**
     * @brief Construct a new Rotary Encoder instance.
     */
    RotaryEncoder() : encoderDir(DIR_NONE), clkPin(ClkPin), dtPin(DtPin), state(R_START)
    {
      thisEncoder = this;
    }

    /**
     * @brief Prepare encoder for use.
     * 
     * @param isEnablePullups true to enable pullup resistors on the
     *            specified pins.
     * @param isPolled true if the pins will be processed by polling
     *            in your main loop. false to use interrupt driven
     *            processing. Default is false.
     */
    void begin(bool isEnablePullups, bool isPolled = false) {
      if (isEnablePullups) {
        pinMode(clkPin, INPUT_PULLUP);
        pinMode(dtPin, INPUT_PULLUP);
      }
      else {
        pinMode(clkPin, INPUT);
        pinMode(dtPin, INPUT);
      }
      if (!isPolled) {
        attachInterrupt(digitalPinToInterrupt(clkPin), encoderChange, CHANGE);
        attachInterrupt(digitalPinToInterrupt(dtPin), encoderChange, CHANGE);
      }
    }

    /**
     * @brief Check the encoder pins to determine if the encoder has been
     *  turned.
     * 
     * @return unsigned char returns an indication of whether the encoder
     *  has been turned and, if so, in which direction. One of DIR_NONE,
     *  DIR_CW or DIR_CCW.
     */
    unsigned char process() {
      // Grab state of input pins.
      unsigned char pinstate = (digitalRead(dtPin) << 1) | digitalRead(clkPin);
      // Determine new state from the pins and state table.
      state = ttable[state & 0xf][pinstate];
      // Return emit bits, ie the generated event.
      return state & 0x30;
    }

    /**
     * @brief Interrupt handler.
     * 
     * This approach will still lose turn events if they are not being processed
     * by the main loop quickly enough.
     */
    static void IRAM_ATTR encoderChange() {
      // Find out which direction the encoder was turned.
      unsigned char newEncoderDir = thisEncoder->process();
    
      // If a turn has been detected and if the last encoder turn
      // has been processed...
      if (DIR_NONE != newEncoderDir && DIR_NONE == thisEncoder->encoderDir) {
        // ...save the encoder event for processing.
        thisEncoder->encoderDir = newEncoderDir;
      }
    }

    /**
     * @brief For interrupt driven processing, get the most recent encoder
     *  turn direction and reset it to DIR_NONE. Therefore, calling this
     *  method a second time before another turn has occurred will return DIR_NONE.
     * 
     * You can also access the encoder direction directly if you require
     * more complex processing of turn events.
     * 
     * @return unsigned char One of DIR_NONE, DIR_CW or DIR_CCW
     */
    unsigned char getAndResetEncoderDir() {
      unsigned char lastDir = encoderDir;
      encoderDir = DIR_NONE;
      return lastDir;
    }

    volatile unsigned char encoderDir;

  private:
    unsigned char clkPin;
    unsigned char dtPin;
    unsigned char state;
    static RotaryEncoder<DtPin, ClkPin>* thisEncoder;
};

template<unsigned char DtPin, unsigned char ClkPin> RotaryEncoder<DtPin, ClkPin>* RotaryEncoder<DtPin, ClkPin>::thisEncoder = nullptr;

#endif  // ROTARYENCODER_H