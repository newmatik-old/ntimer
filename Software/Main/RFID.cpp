/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        RFID.cpp
; * Dependencies:    Header (.inc) files if applicable, see below
; * Processor:       ATMEGA2560
; * Compiler:        Arduino IDE
; *
 * REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author           Date      Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Juan Manuel Garin      10/08/18      First release of source file
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*  Code Description:
*  This object library manage the RFID RC522.
*  It implements a interrupt service routine for detecting when is necessary
*  interrogate the device
 *************************************************************************/

#include "RFID.h"

//************************ Globals *************************//
MFRC522 RFID_MRFC522(RFID_SS_PIN, RFID_RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key Key;
//bool New_Data;
/*!
 * Object constructor:
 *					This function initialize the pins and configure
 *					the RFID RC522 library
 * @param : - 
 * @return: -
*/
RFID::RFID()
{
//  New_Data = false;                     // Clear the new data flag
  RFID_MRFC522.PCD_Init();              // Init MFRC522 card

  #ifdef ENABLE_SERIAL_DEBUG
    Serial.print("Ver: 0x");
    byte readReg = RFID_MRFC522.PCD_ReadRegister(RFID_MRFC522.VersionReg);  //read and printout the MFRC522 version (valid values 0x91 & 0x92)
    Serial.println(readReg, HEX);
  #endif

  RFID_MRFC522.PCD_SetAntennaGain(RFID_MRFC522.RxGain_max);

  ActivateRec();                        // Activate the reception
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.println("End RFID setup");
  #endif
}
/*!
 * The function inform if there is new collected data from RFID card
 * 
 * @param : - 
 * @return: True or False
*/
bool RFID::Is_New_RFID_Card()
{
  if(RFID_MRFC522.PICC_IsNewCardPresent()) // If no RFID card recognized
  {
    RFID_MRFC522.PICC_ReadCardSerial();                           //read the tag data 
   /* #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("RFID Present"); 
    #endif*/
    return true;
  }
  else
  {
    return false;
  }
}
/*!
 * This function return the last value of UID readed
 * 
 * @param : - 
 * @return: value of UID in ulong variable
*/
unsigned long RFID::Read_RFID_Data()
{
  UID_unsigned =  RFID_MRFC522.uid.uidByte[0] << 24;
  UID_unsigned += RFID_MRFC522.uid.uidByte[1] << 16;
  UID_unsigned += RFID_MRFC522.uid.uidByte[2] <<  8;
  UID_unsigned += RFID_MRFC522.uid.uidByte[3];

  /*#ifdef ENABLE_SERIAL_DEBUG
    Serial.println();
    Serial.println("UID Unsigned int"); 
    Serial.println(UID_unsigned);
  #endif*/
//  New_Data = false;                                              // Down the flag of new data
  ActivateRec();
  return UID_unsigned;
}
/*!
 * The function sending to the MFRC522 the needed commands to activate the reception
 * 
 * @param : - 
 * @return: -
*/
void RFID::ActivateRec()
{
  RFID_MRFC522.PCD_WriteRegister(RFID_MRFC522.FIFODataReg, RFID_MRFC522.PICC_CMD_REQA);
  RFID_MRFC522.PCD_WriteRegister(RFID_MRFC522.CommandReg, RFID_MRFC522.PCD_Transceive);
  RFID_MRFC522.PCD_WriteRegister(RFID_MRFC522.BitFramingReg, 0x87);
}
/*!
 * The function sending to the MFRC522 the needed commands to activate the reception
 * 
 * @param : - 
 * @return: -
*/
void RFID::Read_Card()
{
  if(RFID_MRFC522.PICC_IsNewCardPresent()) // If no RFID card recognized
  {
    #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("RFID Present"); 
    #endif
  }
  if (RFID_MRFC522.PICC_ReadCardSerial()) //Since a PICC placed get Serial and continue
  {   
    #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("RFID Readed"); 
    #endif
    Read_RFID_Data();
  }
  ActivateRec();
  RFID_MRFC522.PCD_WriteRegister(RFID_MRFC522.ComIrqReg, 0x7F);
 // delay(100);
 /* byte status;
  byte data[50];
  byte serial[5];
  int i, j, pos;

  // Send a general request out into the aether. If there is a tag in
  // the area it will respond and the status will be MI_OK.
  status = RFID_MRFC522.requestTag(MF1_REQIDL, data);

  if (status == MI_OK) 
  {
    Serial.println("Tag detected.");
    Serial.print("Type: ");
    Serial.print(data[0], HEX);
    Serial.print(", ");
    Serial.println(data[1], HEX);
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) // Wenn if no RFID senderwas selected
  {
    return; // jump back and continue loop
  }*/
}
/*!
 * MFRC522 interrupt serving routine
 * 
 * @param : - 
 * @return: -
*/
/*void IRQ_ReadCard() 
{
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.print("RFID Interrupt");
  #endif
  RFID_MRFC522.PICC_ReadCardSerial();                           //read the tag data  

  RFID_MRFC522.PCD_WriteRegister(RFID_MRFC522.ComIrqReg, 0x7F); // Clear the pending interrupt flag in RC522
  RFID_MRFC522.PICC_HaltA();                                    //Instructs a PICC in state ACTIVE(*) to go to state HALT.

  New_Data = true;                                              // Up the flag of new data
}*/
