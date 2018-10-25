/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        SD_Card.h
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
*  This object library manage the micro SD card
 *************************************************************************/
#ifndef SD_Card_h
#define SD_Card_h

#include "Arduino.h"
#include "Hardware.h"
#include "Display.h"
#include <SPI.h>
#include <SD.h>
#include "src\Ethernet2\Ethernet2.h"

#define BUFFER_LENGTH  250

struct file_Data
{
   bool Dhcp;
   char ServerName[STRING_LENGTH];
   char Secret[STRING_LENGTH];
   char Key[STRING_LENGTH];
   bool OnlineUpdate;
   bool SD_Update;
   IPAddress Ip;
   IPAddress Dns;
   IPAddress Gateway;
};
typedef struct file_Data File_Data;

class SD_Card
{
  public:
	  SD_Card();
    char SD_Status();
    bool Is_DHCP_Enabled();
    IPAddress Ip_Address();
    IPAddress Dns_Address();
    IPAddress Gateway_Address();
    char* Read_ServerName();
    char* Read_Secret();
    char* Read_Key();
    bool Is_OnlineUpdate_Enabled();
    bool Is_SD_Update_Enabled();
  private:
    bool Read_Line(char *Data);
    void Read_Config_File();
    char SD_Status_Var;
    File MyFile;
    File_Data Readed_Data;
};

#endif
