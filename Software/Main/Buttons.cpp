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
*  This object library manage input buttons status and actions.
*  It uses interrupt service routines which change the status flags
*  that are requested by main program
 *************************************************************************/

#include "Buttons.h"

//************************ Globals *************************//
bool Come_Flag;
bool Go_Flag;
bool Enable_Buttons;
void Button_Come_Change();
void Button_Go_Change();
/*!
 * Object constructor:
 *					This function initialize the pins and configure
 *					the interrupts for the button pins
 * @param : - 
 * @return: -
*/
Buttons::Buttons()
{
  Come_Flag = false;
  Go_Flag = false;
  Enable_Buttons = false;
  pinMode(COME_BUTTON_PIN, INPUT);  //DIG19_INT2
  attachInterrupt(digitalPinToInterrupt(COME_BUTTON_PIN), Button_Come_Change, FALLING);
	pinMode(GO_BUTTON_PIN, INPUT);    //DIG18_INT3
  attachInterrupt(digitalPinToInterrupt(GO_BUTTON_PIN), Button_Go_Change, FALLING);
}
/*!
 * Function that returns the state of the Come button flag
 *
 * @param : - 
 * @return: true or false
*/
bool Buttons::Is_Come_Button_Pressed()
{
  if(Come_Flag)
  {
    Come_Flag = false;
    return true;
  }
  else
  {
    return false;
  } 
}
/*!
 * Function that returns the state of the Go button flag
 *
 * @param : - 
 * @return: true or false
*/
bool Buttons::Is_Go_Button_Pressed()
{
  if(Go_Flag)
  {
    Go_Flag = false;
    return true;
  }
  else
  {
    return false;
  } 
}
/*!
 * Function that enable the actions on the buttons
 *
 * @param : - 
 * @return: -
*/
void Buttons::EnableButtons()
{
  Enable_Buttons = true;
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.println("Buttons Enabled");
  #endif
}
/*!
 * Function that disable the actions on the buttons
 *
 * @param : - 
 * @return: -
*/
void Buttons::DisableButtons()
{
  Enable_Buttons = false;
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.println("Buttons Disabled");
  #endif
}
/*!
 * Function that change the status flag of the Come button change.
 *
 * @param : - 
 * @return: -
*/
void Button_Come_Change()
{
  if(Enable_Buttons)
  {
    Come_Flag = true;
    #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("Come Button Pressed");
    #endif
  }
}
/*!
 * Function that change the status flag of the Go button change.
 *
 * @param : - 
 * @return: -
*/
void Button_Go_Change()
{
  if(Enable_Buttons)
  {
    Go_Flag = true;
    #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("Go Button Pressed");
    #endif
  }
}
