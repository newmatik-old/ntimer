/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        Main.ino
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
*  This is the main file of the project.
 *************************************************************************/
 
#include "Beeper.h"
#include "Buttons.h"
#include "Display.h"
#include "RFID.h"
#include "Eth_Client.h"
#include "SD_Card.h"
#include "Hardware.h"
#include "src\RFID\MFRC522.h"
#include "src\Ticker\Ticker.h"

//Object libraries
Beeper Beep;
Buttons Button;
RFID *RFID_Tag;
Display *I2C_Display;
Eth_Client *Ethernet_Client;
SD_Card *SDCard;

//************************ Globals *************************//
void Count_Down_Function();
Ticker Timer_CountDown(Count_Down_Function, 1000, 5, MILLIS);  // five times, 1s
void Manage_Status_Falgs();
unsigned long RFID_UId;
const String softwareVersion = "1.8";
const int Led =13;
bool Some_Error;
char Data;
int Count_Down, Count_Valid, Count_Error;

typedef enum
{
    SM_START,
    SM_COME_RFID,
    SM_PUNCHING_COME_IN,
    SM_GO_RFID,
    SM_PUNCHING_GO_IN,
    SM_RESULT
} SM_CONTROL;
static SM_CONTROL smControl=SM_START;
/*!
 * Initilization of the uC pins, hardware and libraries.
*/
void setup() 
{
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.begin(115200);                                     // Initialize serial communications with the PC
    Serial.println("Opening serial connection for debugging monitor");
    Serial.println("Software version: " + softwareVersion);
  #endif
  Some_Error = false;
  Count_Down = COUNTDOWN_SEC;
  Count_Valid = COUNT_VALID_SEC;
  /******************** Working Blinking Led **************/
  pinMode(Led,OUTPUT);
  /************************* Display LCD ******************/
  I2C_Display = new Display();
  I2C_Display->Print_Msg(INITIALIZING_MSG);
  tone(BEEPER_PIN, BEEPER_FREQUENCY); // Send 4KHz sound signal...
  delay(2000);
  noTone(BEEPER_PIN);
  I2C_Display->Set_Brightness(NORMAL_BACKLIGHT_BRIGHTNESS);
  /******************* SPI for all devices ****************/
  SPI.begin();                          // Init SPI bus 
  /************************ SD Card ***********************/
  SDCard = new SD_Card();
  /************************ Ethernet **********************/
  Ethernet_Client = new Eth_Client();
  /************************* RFID *************************/
  RFID_Tag = new RFID();
  /*************** Enable the button actions **************/
  Button.EnableButtons();            
}
/*!
 * Main loop of the software
*/
void loop() 
{
 // Check the Server Status
  if(Ethernet_Client->Ethernet_Status() != STATUS_ETH_CONNECTED)
  {
    Ethernet_Client->Server_Connect();
  }
  else
  {
    //State machine
    switch(smControl)
    {
      case SM_START:
          I2C_Display->Print_Msg(WAITING_MSG);
          if(Button.Is_Come_Button_Pressed())
          {
            smControl = SM_COME_RFID;
            Button.DisableButtons();            // Disable the button actions
            I2C_Display->Set_CountDown(Count_Down);
            I2C_Display->Print_Msg(COME_RFID_MSG);
            Count_Down = COUNTDOWN_SEC; 
            Timer_CountDown.start();
            Beep.Short_Beep();
          }  
          if(Button.Is_Go_Button_Pressed())
          {
            smControl = SM_GO_RFID;
            Button.DisableButtons();            // Disable the button actions
            I2C_Display->Set_CountDown(Count_Down);
            I2C_Display->Print_Msg(GO_RFID_MSG);
            Count_Down = COUNTDOWN_SEC; 
            Timer_CountDown.start();
            Beep.Short_Beep();
          }  
          break; 
      //********************** COME state machine **************/
      case SM_COME_RFID:
          I2C_Display->Set_CountDown(Count_Down);
          I2C_Display->Print_Msg(COME_RFID_MSG);
          if(Count_Down == 0)                  // If in 5 secconds nothing happens restart the cycle
          {
            smControl = SM_START;
            Timer_CountDown.stop();
            Count_Down = COUNTDOWN_SEC; 
            Button.EnableButtons();            // Enable the button actions 
          }
          if(RFID_Tag->Is_New_RFID_Card())     // If a RFID is pressented
          {
            RFID_UId = RFID_Tag->Read_RFID_Data();
           // Beep.Long_Beep();
            Timer_CountDown.stop();
            Count_Down = COUNTDOWN_SEC; 
            smControl = SM_PUNCHING_COME_IN; 
            I2C_Display->Print_Msg(RFID_CONNECTING);
          }
          break;
      case SM_PUNCHING_COME_IN:
          // Send RFID_UId to the server
          Ethernet_Client->PunchIn(RFID_UId); 
          Count_Valid = COUNT_VALID_SEC;
          Timer_CountDown.start();
          smControl = SM_RESULT;
          break;
      //********************** GO state machine **************/
      case SM_GO_RFID:
          I2C_Display->Set_CountDown(Count_Down);
          I2C_Display->Print_Msg(GO_RFID_MSG);
          if(Count_Down == 0)                  // If in 5 secconds nothing happens restart the cycle
          {
            smControl = SM_START;
            Timer_CountDown.stop();
            Count_Down = COUNTDOWN_SEC; 
            Button.EnableButtons();            // Enable the button actions 
          }
          if(RFID_Tag->Is_New_RFID_Card())    // If a RFID is pressented
          {
            RFID_UId = RFID_Tag->Read_RFID_Data();
         //   Beep.Long_Beep();
            Timer_CountDown.stop();
            Count_Down = COUNTDOWN_SEC; 
            smControl = SM_PUNCHING_GO_IN; 
            I2C_Display->Print_Msg(RFID_CONNECTING);
          }
          break;
      case SM_PUNCHING_GO_IN:
          // Send RFID_UId to the server
          Ethernet_Client->PunchOut(RFID_UId);
          Count_Valid = COUNT_VALID_SEC;
          Timer_CountDown.start();
          smControl = SM_RESULT;
          break;
      //********************** Results of the transaction **************/
      case SM_RESULT:
          if(Count_Valid <= 0)                  // If in 5 secconds nothing happens restart the cycle
          {
            smControl = SM_START;
            Timer_CountDown.stop();
            Count_Valid = COUNT_VALID_SEC;
            Count_Down = COUNTDOWN_SEC;   
            Button.EnableButtons();            // Enable the button actions 
          }
          break;
    }
  }
  Timer_CountDown.update();
  delay(50);
  //**************** Working Led *******************/
 /*digitalWrite(Led,HIGH);
  delay(500);
  digitalWrite(Led,LOW);
  delay(500); */
 // RFID_Tag->Read_Card();
}
/*!
 * Function that make a count down over variables
 * used to display each one seccond a count down value.
 *
 * @param : - 
 * @return: -
*/
void Count_Down_Function()
{
  Count_Down--;
  Count_Valid--;
}

