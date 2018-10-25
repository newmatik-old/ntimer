/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        RFID.h
; * Dependencies:    Header (.inc) files if applicable, see below
; * Processor:       ATMEGA2560
; * Compiler:        Arduino IDE
; *
 * REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author 					Date			Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Juan Manuel Garin      10/08/18  		First release of source file
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*  Code Description:
*  This object library manage the RFID RC522.
*  It implements a interrupt service routine for detecting when is necessary
*  interrogate the device
 *************************************************************************/
#ifndef RFID_h
#define RFID_h

#include "Arduino.h"
#include "Hardware.h"
#include <SPI.h>
#include "src\RFID\MFRC522.h"

class RFID
{
  public:
	  RFID();
    bool Is_New_RFID_Card();
    unsigned long Read_RFID_Data();
    void Read_Card();
  private: 
    void ActivateRec();
    byte RegVal;
    unsigned long UID_unsigned;
};

#endif
