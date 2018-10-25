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
*  This object library manage input buttons status and actions.
*  It uses interrupt service routines which change the status flags
*  that are requested by main program
 *************************************************************************/
#ifndef Buttons_h
#define Buttons_h

#include "Arduino.h"
#include "Hardware.h"

class Buttons
{
  public:
	  Buttons();
    bool Is_Come_Button_Pressed();
    bool Is_Go_Button_Pressed();
    void EnableButtons();
    void DisableButtons();
  private: 
};

#endif
