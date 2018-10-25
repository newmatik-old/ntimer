/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        Beeper.cpp
; * Dependencies:    Header (.inc) files if applicable, see below
; * Processor:       ATMEGA2560
; * Compiler:        Arduino IDE
; *
 * REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author 					Date			Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Juan Manuel Garin      08/08/18  		First release of source file
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*  Code Description:
*  This object library manage the bepper of Newmatik Ntimer.
*  It uses a timer for making the beep and dont freeze the uC util this time.
 *************************************************************************/

#include "Arduino.h"
#include "Beeper.h"

//************************ Globals *************************//
Ticker Timer_1s(Stop_Beep, 0, 1000);  // once, 1s
Ticker Timer_2s(Stop_Beep, 0, 2000);    // once, 2s
bool Beeping;

/*!
 * Object constructor:
 *					This function initialize the pins and configure
 *					the PWM for working in 4KHz
 *	Output A frequency: 16 MHz / 8 / (249+1) / 2 = 4000Hz
 *  Output A duty cycle: 50%
 * @param : - 
 * @return: -
*/
Beeper::Beeper()
{
/*	TCCR1A = _BV(COM1A0) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);   // Configure the timer registers
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1A = 249;*/
  pinMode(BEEPER_PIN, OUTPUT);  //DIG11/PWM -> OC1A_PB5
	Beeping = false;
}
/*!
 * Function that enables the beeper until half second.
 *
 * @param : - 
 * @return: -
*/
void Beeper::Short_Beep()
{
  tone(BEEPER_PIN, BEEPER_FREQUENCY); // Send 4KHz sound signal...
  delay(SHORT_BEEP_TIME);
  noTone(BEEPER_PIN);
/*	Timer_1s.stop();
	Timer_1s.start();
	Beeping=true;*/
}
/*!
 * Function that enables the beeper until 1 second.
 *
 * @param : - 
 * @return: -
*/
void Beeper::Long_Beep()
{	
	tone(BEEPER_PIN, BEEPER_FREQUENCY); // Send 4KHz sound signal...
  delay(LONG_BEEP_TIME);
  noTone(BEEPER_PIN);	
  /*Timer_2s.stop();
	Timer_2s.start();
	Beeping=true;*/
}
/*!
 * Function that makes two beeps with different frequencies
 *
 * High->Low
 * @param : - 
 * @return: -
*/
void Beeper::High_Low_Beep()
{  
  tone(BEEPER_PIN, HIGH_BEEPER_FREQUENCY);
  delay(SHORT_BEEP_TIME);
  tone(BEEPER_PIN, LOW_BEEPER_FREQUENCY); 
  delay(SHORT_BEEP_TIME);
  noTone(BEEPER_PIN); 
}
/*!
 * Function that makes two beeps with different frequencies
 * Low->High
 *
 * @param : - 
 * @return: -
*/
void Beeper::Low_High_Beep()
{  
  tone(BEEPER_PIN, LOW_BEEPER_FREQUENCY);
  delay(SHORT_BEEP_TIME);
  tone(BEEPER_PIN, HIGH_BEEPER_FREQUENCY); 
  delay(SHORT_BEEP_TIME);
  noTone(BEEPER_PIN); 
}
/*!
 * Function that returns the state of beeper
 *
 * @param : - 
 * @return: true or false
*/
bool Beeper::Is_Beeping()
{
	return Beeping;
}
/*!
 * Function that update the beeper timer status
 *
 * @param : - 
 * @return: -
*/
void Beeper::Update_Beeping_Time()
{
	Timer_1s.update();
	Timer_2s.update();
}
/*!
 * Function that stops the beeper
 *
 * @param : - 
 * @return: -
*/
void Stop_Beep()
{
	Beeping=false;
	noTone(BEEPER_PIN);
}

