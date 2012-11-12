#include "TestSoftPWM.h"

SoftPWM led2(4);
SoftPWM led3(5);
SoftPWM led4(6);

TestSoftPWM::TestSoftPWM()
{

}

void TestSoftPWM::setup()
{
    SoftPWM::SetFrequency(1);
  //SoftPWM2::SetPeriod(2000000UL);

//	pinMode(5, OUTPUT);
//  digitalWrite(5, HIGH);
//	pinMode(6, OUTPUT);
//  digitalWrite(6, LOW);


	// initialize the digital pin as an output.
	// Pin 13 has an LED connected on most Arduino boards:
	pinMode(2, OUTPUT);
//	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	//digitalWrite(4, HIGH);   // set the LED on
//	digitalWrite(3, HIGH);   // set the LED on
//	digitalWrite(2, LOW);   // set the LED on

  led2.setPWM(40);
  led3.setPWM(40);
  //delay(500);
  led4.setPWM(40);

  delay(100);
  led3.run();
  delay(300);
  led4.setPWM(40);
  //led3.invertPWM();
}

void TestSoftPWM::run()
{
  led2.run();
  led3.run();
  led4.run();
}
