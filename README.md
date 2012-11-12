# SoftPWM Library (version: 2.0)

> &copy; 2012-2012 Scott Daniels <provideyourown.com>
> under GNU General Public License

The SoftPWM Library permits you to implement pulse width modulation
in software on any pin. Unlike other implementations, everything is
neatly encapsulated in a single class.

## Usage example

    kLedPin = 13;

    SoftPWM led(kLedPin);

    void setup()
    {
      led.setPWM(25); // set duty cycle to 25%
    }

    void loop()
    {
      led.run(); // turns led off & on as required
    }

## Methods

**Construction & Configuration**

  SoftPWM(byte pinNo);

  void setPWM(byte percent); // fraction * 100, e.g. for 97%, enter 97

  void invertPWM(); // if the duty cycle should be reversed, e.g. 95% becomes 5%

  // turn pwm on & off
  void enable();
  void disable();

  // static methods to set the base frequency/period. Default = 1,000 microseconds
  static void SetFrequency(unsigned long freq = 1000);
  static void SetPeriod(unsigned long period = 1000); // microsecs

**Pulse the LED**

  void run(); // call this once each time in your loop() func

