/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        Beeper.h
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
*  This object library manage the bepper of Newmatik Ntimer
*  It uses a timer for making the beep and dont freeze the uC util this time.
 *************************************************************************/
#ifndef Beeper_h
#define Beeper_h

#include "Arduino.h"
#include "src\Ticker\Ticker.h"
#include "Hardware.h"

void Stop_Beep();

class Beeper
{
  public:
	  Beeper();
    void Short_Beep();
	  void Long_Beep();
    void High_Low_Beep();
    void Low_High_Beep();
	  bool Is_Beeping();
	  void Update_Beeping_Time();
  private: 
};

#endif
