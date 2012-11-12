/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */


/*  * * * * * * * * * * * * * * * * * * * * * * * * * * *
 SoftPWM Library
 Author: Scott Daniels
 Website: http://provideyourown.com
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
The SoftPWM Library permits you to implement pulse width modulation
in software on any pin. Unlike other implementations, everything is
neatly encapsulated in a single class.

See README.md for usage example
*/

#ifndef SOFTPWM_H
#define SOFTPWM_H

#include <Arduino.h>

class SoftPWM {
public:
  SoftPWM(byte pinNo);

  void run(); // call this once each time in your loop() func

  void setPWM(byte percent); // fraction * 100, e.g. for 97%, enter 97

  void enable();
  void disable();

  void invertPWM() { mInvertPWM = true; }


  static void SetFrequency(unsigned long freq = 1000);
  static void SetPeriod(unsigned long period = 1000); // microsecs

protected:
  bool mInvertPWM; // when true, 95% would actually be 5%. Used when the LED should be on when the pin is grounded.
  byte mPort;
  byte mBitMask;
  byte mPercentPWM;
  unsigned long mExpirationTime;

  bool modulationOn() const;
  byte getPinMode() const;
  byte getPinState() const;

  void setPinMode(byte mode);
  void setPinState(byte state);
  void togglePinState();

  void setNextExpiration();

  static unsigned long Interval;
};

#endif // SOFTPWM2_H
