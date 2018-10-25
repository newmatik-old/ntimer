/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        Display.cpp
; * Dependencies:    Header (.inc) files if applicable, see below
; * Processor:       ATMEGA2560
; * Compiler:        Arduino IDE
; *
 * REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author           Date      Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Juan Manuel Garin      09/08/18      First release of source file
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*  Code Description:
*  This object library manage the display, the I2C connection and 
*  the showed messages
 *************************************************************************/

#include "Display.h"

//************************ Globals *************************//
ST7036 LCD = ST7036 (2,20,0x78,BACKLIGHT_DISPLAY);  // set the LCD address to 0x78 for a 20 chars and 2 line display
extern String softwareVersion = "1.8";
extern Eth_Client *Ethernet_Client;
extern SD_Card *SDCard;
byte CountDown;
char *pChar;
/*!
 * Object constructor:
 *					This function initialize the pins and configure
 *					the display library
 * @param : - 
 * @return: -
*/
Display::Display()
{
  pinMode(RST_DISPLAY,OUTPUT);
  digitalWrite(RST_DISPLAY,HIGH);                 // Unreset the display
  LCD.init();
  LCD.setContrast(10);
  LCD.setBacklight(INITIAL_BACKLIGHT_BRIGHTNESS);
  CountDown = COUNTDOWN_SEC;                      // Variable used for count down
}
/*!
 * Function that writes a message in the display using
 * the parameter Msg_Descriptor as reference for the message
 *
 * @param Msg_Descriptor: Name=Number of the message 
 * @return: -
*/
void Display::Print_Msg(char Msg_Descriptor)
{
  LCD.clear ();
  switch(Msg_Descriptor)
  {
    case WELCOME_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Welcome");
      break;
    case INITIALIZING_MSG:
      LCD.setCursor(0, 0);
      LCD.print("ntimer software v" + softwareVersion);
      LCD.setCursor (1, 0 );
      LCD.print("Initializing"); 
      break;
    case SD_FAILS_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Initialization Fails");
      LCD.setCursor (1, 0 );
      LCD.print("SD Card Error");
      delay(3000); 
      break;
    case SD_NO_FILE_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Initialization Fails");
      LCD.setCursor (1, 0 );
      LCD.print("SD File Error"); 
      delay(3000);
      break;
    case ETH_DHCP_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Ethernet Connecting");
      LCD.setCursor (1, 0 );
      LCD.print("DHCP Enabled"); 
      delay(2000);
      break;
    case ETH_DHCP_ERROR_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Ethernet Connecting");
      LCD.setCursor (1, 0 );
      LCD.print("DHCP Failed");
      delay(2000); 
      break;
    case ETH_IP_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Ethernet Connected");
      LCD.setCursor (1, 0 );
      LCD.print("IP: "); 
      LCD.setCursor (1, 4 );
      LCD.print(Ethernet_Client->LocalIP());
      delay(2000);
      break;
    case ETH_SERVER_CONNECTING_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Server Connecting");
      delay(2000);
      break;
    case ETH_SERVER_CONNECTED_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Server Connected");
      LCD.setCursor (1, 0 );
      LCD.print(SDCard->Read_ServerName());
      delay(2000);
      break;
    case ETH_SERVER_ERROR_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Server Error");
      LCD.setCursor (1, 0 );
      LCD.print(SDCard->Read_ServerName());
      delay(2000);
      break;
    case WAITING_MSG:
      LCD.setCursor(0, 0);
      LCD.print("WAITING");
      LCD.setCursor (1, 0 );
      LCD.print("Press COME or GO");
      break;
    case COME_RFID_MSG:
      LCD.setCursor(0, 0);
      LCD.print("COME");
      LCD.setCursor (1, 0 );
      LCD.print("Present RFID Card ( )");
      LCD.setCursor (1, 18);
      LCD.print(CountDown);
      break;
    case COME_VALID_DATA_MSG:
      LCD.setCursor(0, 0);
      LCD.print("COME");
      LCD.setCursor(0, 10);
      pChar=Ethernet_Client->Read_In_Time();
      pChar+=11;
      LCD.print(pChar);
      LCD.setCursor (1, 0 );
      LCD.print(Ethernet_Client->Read_Employee_Name());
      break;
    case GO_RFID_MSG:
      LCD.setCursor(0, 0);
      LCD.print("GO");
      LCD.setCursor (1, 0 );
      LCD.print("Present RFID Card ( )");
      LCD.setCursor (1, 18);
      LCD.print(CountDown);
      break;
    case GO_VALID_DATA_MSG:
      LCD.setCursor(0, 0);
      LCD.print("DURATION");
      LCD.setCursor(0, 12);
      LCD.print(Ethernet_Client->Read_Duration());
      LCD.setCursor (1, 0 );
      LCD.print(Ethernet_Client->Read_Employee_Name());
      break;
    case RFID_ERROR_TAG_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Error");
      LCD.setCursor (1, 0 );
      LCD.print("RFID tag unknown");
      break;
    case EMPLOYEE_ERROR_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Error");
      LCD.setCursor (1, 0 );
      LCD.print("Employee not active");
      break;
    case ALREADY_ERROR_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Error");
      LCD.setCursor (1, 0 );
      LCD.print("Already punched in");
      break;
    case NO_PUNCHIN_ERROR_MSG:
      LCD.setCursor(0, 0);
      LCD.print("Error");
      LCD.setCursor (1, 0 );
      LCD.print("Not punched in");
      break;
    case RFID_CONNECTING:
      LCD.setCursor(0, 0);
      LCD.print("Connecting...");
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }
}
/*!
 * Function that set the backlight brigthness.
 * The value must be between 0-255
 *
 * @param Value: brightness intensity
 * @return: -
*/
void Display::Set_Brightness(char Value)
{
  LCD.setBacklight(Value);
}
/*!
 * Function that load the value of coutdown
 *
 * @param Value: brightness intensity
 * @return: -
*/
void Display::Set_CountDown(char Value)
{
  CountDown = Value;
}

