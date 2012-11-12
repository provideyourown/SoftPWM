#include "SoftPWM.h"

#include <util/atomic.h>

//#define debug

#ifdef debug
#include "../../libraries/Streaming/Streaming.h"
#endif


unsigned long SoftPWM::Interval = 100;

void SoftPWM::SetFrequency(unsigned long freq) { SoftPWM::Interval = 1000000UL / (unsigned long)freq; }
void SoftPWM::SetPeriod(unsigned long period) { SoftPWM::Interval = period; } // microsecs

SoftPWM::SoftPWM(byte pinNo)
  : mInvertPWM(false), mPort(0), mBitMask(0), mPercentPWM(0), mExpirationTime(0)
{
  mPort = digitalPinToPort(pinNo);
  mBitMask = digitalPinToBitMask(pinNo);
}

void SoftPWM::run()
{
  if (!modulationOn()) return; // disabled, return as to not take any more processing time

  // handles rollover, see - http://arduino.cc/playground/Code/TimingRollover
  if (((long)(micros() - mExpirationTime)) > 0) // time to execute task
  {
    togglePinState(); // togglePinState changes our time interval, so we do it first
    setNextExpiration(); // set time for next task
  }
  // else wait till next time
}

// fraction * 100, e.g. for 97%, enter 97
void SoftPWM::setPWM(byte percent)
{
  setPinMode(OUTPUT); // make sure pin is set to output

  mPercentPWM = percent;

  if (mInvertPWM) percent = 100 - percent; // handle case when pin is ground for LED on state

  if (percent <= 0) // turn the pin off and disable to task to save processing time
  {
    setPinState(LOW); //digitalWrite(mPin, 0);
  }
  else if (percent >= 100)
  {
    setPinState(HIGH); //digitalWrite(mPin, 1);
  }
  else
    setNextExpiration();
}

void SoftPWM::enable()
{
  setPinMode(OUTPUT); // re-enable the port
  setPWM(mPercentPWM); // turn it back on
}

void SoftPWM::disable()
{
  // first turn it off
  byte percent = mPercentPWM; // save value
  setPWM(0); // turn off output
  mPercentPWM = percent; // restore value

  // now disable the port
  setPinMode(INPUT);
}

bool SoftPWM::modulationOn() const
{
  return getPinMode() && (mPercentPWM > 0) && (mPercentPWM < 100);
}

void SoftPWM::setNextExpiration()
{
  bool onState = getPinState() ^ mInvertPWM; // XOR - invert pin state by invertPWM
//  if (mInvertPWM)
//    onState = !onState;
  #ifdef debug
  Serial << "on? " << onState << endl;
  #endif
  // get the current on state and determine which part of duty cycle to use
  byte percent = (onState)? mPercentPWM : (100-mPercentPWM);

  unsigned long curr = micros();
  unsigned long interv = ((unsigned long)percent) * SoftPWM::Interval / 100UL;
  mExpirationTime = curr + interv;

  #ifdef debug
  Serial << "percent: " << percent << " Interval: " << SoftPWM::Interval << " calc interv: " << interv << endl;
  Serial << "currentTime: " << curr << " expirationTime: " << mExpirationTime << endl;
  #endif
}

byte SoftPWM::getPinMode() const
{
	volatile uint8_t* reg = portModeRegister(mPort);
  return *reg & mBitMask;
}

byte SoftPWM::getPinState() const
{
	volatile uint8_t *out = portOutputRegister(mPort);
  return *out & mBitMask;
}

void SoftPWM::setPinMode(byte mode)
{
	if (mPort == NOT_A_PIN) return;
	volatile uint8_t* reg = portModeRegister(mPort);

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) // see http://www.nongnu.org/avr-libc/user-manual/group__util__atomic.html
  {
    if (mode == INPUT)
      *reg &= ~mBitMask;
    else
      *reg |= mBitMask;
  }
}

void SoftPWM::setPinState(byte state)
{
	if (mPort == NOT_A_PIN) return;
	volatile uint8_t *out = portOutputRegister(mPort);

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) // see http://www.nongnu.org/avr-libc/user-manual/group__util__atomic.html
  {
    if (state == LOW)
      *out &= ~mBitMask;
    else
      *out |= mBitMask;
  }
}

void SoftPWM::togglePinState()
{
	if (mPort == NOT_A_PIN) return;
	volatile uint8_t *out = portOutputRegister(mPort);

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) // see http://www.nongnu.org/avr-libc/user-manual/group__util__atomic.html
  {
    *out ^= mBitMask; // XOR toggles the bit
  }
}
