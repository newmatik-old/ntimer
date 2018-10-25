/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        Eth_Client.h
; * Dependencies:    Header (.inc) files if applicable, see below
; * Processor:       ATMEGA2560
; * Compiler:        Arduino IDE
; *
 * REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author 					Date			Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Juan Manuel Garin      13/08/18  		First release of source file
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*  Code Description:
*  This object library manage the ethernet client using the W5500 as interface
 *************************************************************************/
#ifndef Eth_Client_h
#define Eth_Client_h

#include "Arduino.h"
#include "Hardware.h"
#include "Display.h"
#include "SD_Card.h"
#include "Beeper.h"
#include <SPI.h>
#include "src\Ethernet2\Ethernet2.h"
#include "src\TextFinder\TextFinder.h"
#include "src\HTTPClient\ArduinoHttpClient.h"

#define BUFFER_LENGTH       350
#define DELAY_HTTP          250
/*************** Keywords *****************/
#define KEY_LOGIN               "Logged In"
#define KEY_STATUS              "status"
#define KEY_ERROR               "error"
#define KEY_ERROR_MSG           "error_message"
#define KEY_SUCCESS             "success"
#define KEY_RFID_ERROR          "RFID Tag unknown."
#define KEY_EMPL_ERROR          "Employee not active."
#define KEY_ALREADY_PUNCHIN_ERROR       "Already punched in."
#define KEY_NO_PUNCHIN_ERROR            "Not punched in."
#define KEY_IN_TIME             "in_time"
#define KEY_EMPL_NAME           "employee_name"
#define KEY_RFID_TAG            "rfid_tag"
#define KEY_OUT_TIME            "out_time"
#define KEY_DURATION            "duration"

struct Transaction
{
   bool Status;
   char Error_Mge[STRING_LENGTH];
   char In_Time[STRING_LENGTH];
   char Out_Time[STRING_LENGTH];
   char Employee_Name[STRING_LENGTH];
   char Duration[STRING_LENGTH];
   char RFIDTag[STRING_LENGTH];
};
typedef struct Transaction Punch;

class Eth_Client
{
  public:
	  Eth_Client();
    bool Is_Incomming_Data();
    void Server_Connect();
    char Ethernet_Status();
    IPAddress LocalIP();
    bool PunchIn(unsigned long RFID_Tag);
    bool PunchOut(unsigned long RFID_Tag);
    char* Read_In_Time();
    char* Read_Out_Time();
    char* Read_Employee_Name();
    char* Read_Duration();
  private:
    char ETH_Status; 
    bool Incomming_Data;
    char ServerName[STRING_LENGTH];
    Punch Punch_Data;
};

#endif
