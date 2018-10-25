/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        Display.h
; * Dependencies:    Header (.inc) files if applicable, see below
; * Processor:       ATMEGA2560
; * Compiler:        Arduino IDE
; *
 * REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author 					Date			Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Juan Manuel Garin      09/08/18  		First release of source file
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*  Code Description:
*  This object library manage the display, the I2C connection and 
*  the showed messages
 *************************************************************************/
#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "Hardware.h"
#include "Eth_Client.h"
#include "SD_Card.h"
#include <Wire.h>
#include "src\LCD_C0220BIZ\ST7036.h"
#include "src\LCD_C0220BIZ\LCD_C0220BiZ.h"

#define WELCOME_MSG               0
#define INITIALIZING_MSG          1
#define SD_FAILS_MSG              2
#define SD_NO_FILE_MSG            3
#define ETH_DHCP_MSG              4
#define ETH_DHCP_ERROR_MSG        5
#define ETH_IP_MSG                6
#define ETH_SERVER_CONNECTING_MSG 7
#define ETH_SERVER_CONNECTED_MSG  8
#define ETH_SERVER_ERROR_MSG      9
#define WAITING_MSG               10
#define COME_RFID_MSG             11
#define COME_VALID_DATA_MSG       12
#define GO_RFID_MSG               13
#define GO_VALID_DATA_MSG         14
#define RFID_ERROR_TAG_MSG        15
#define EMPLOYEE_ERROR_MSG        16
#define ALREADY_ERROR_MSG         17
#define NO_PUNCHIN_ERROR_MSG      18
#define RFID_CONNECTING           19

class Display
{
  public:
	  Display();
    void Set_Brightness(char Value);
    void Print_Msg(char Msg_Descriptor);
    void Print_IP(char Text);
    void Set_CountDown(char Value);
  private: 
};

#endif
