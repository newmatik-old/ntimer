/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        Eth_Client.cpp
; * Dependencies:    Header (.inc) files if applicable, see below
; * Processor:       ATMEGA2560
; * Compiler:        Arduino IDE
; *
 * REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author           Date      Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Juan Manuel Garin      13/08/18      First release of source file
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*  Code Description:
*  This object library manage the ethernet client using the W5500 as interface
 *************************************************************************/

#include "Eth_Client.h"

//************************ Globals *************************//
extern Display *I2C_Display;
extern SD_Card *SDCard;
extern Beeper Beep;
bool New_ETHData;
byte Mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
char Aux_String[BUFFER_LENGTH];
char Token[100];
/*!Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
*/
EthernetClient EthClient;  
HttpClient client = HttpClient(EthClient, "staging.newmatik.com");

/*!
 * Object constructor:
 *					This function initialize the pins and configure
 *					the ethernet library
 * @param : - 
 * @return: -
*/
Eth_Client::Eth_Client()
{
  unsigned long Ulong_Test;
  // Configure chip as 1,1,1 in Pmode pins = All capable, Auto-negotiation enabled
  pinMode(ETH_PMODE0_PIN, OUTPUT);      // Setup the PMODE0 Pin
  pinMode(ETH_PMODE1_PIN, OUTPUT);      // Setup the PMODE1 Pin
  pinMode(ETH_PMODE2_PIN, OUTPUT);      // Setup the PMODE2 Pin
  digitalWrite(ETH_PMODE0_PIN, HIGH);
  digitalWrite(ETH_PMODE1_PIN, HIGH);
  digitalWrite(ETH_PMODE2_PIN, HIGH);

  // Reset the IC
  pinMode(ETH_RST_PIN, OUTPUT);         // Setup the RESET Pin
  digitalWrite(ETH_RST_PIN, HIGH); 
  
  if(SDCard->Is_DHCP_Enabled())
  {
    I2C_Display->Print_Msg(ETH_DHCP_MSG);
    if(Ethernet.begin(Mac) == 0)          // start the Ethernet connection:
    {
      #ifdef ENABLE_SERIAL_DEBUG
        Serial.println("Failed to configure Ethernet using DHCP");
      #endif
      I2C_Display->Print_Msg(ETH_DHCP_ERROR_MSG);
      
      Ethernet.begin(Mac, SDCard->Ip_Address(), SDCard->Dns_Address(), SDCard->Gateway_Address()); // try to configure using IP address instead of DHCP
    }
  }
  else
  {
    Ethernet.begin(Mac, SDCard->Ip_Address(), SDCard->Dns_Address(), SDCard->Gateway_Address()); // try to configure using IP address instead of DHCP
  }
    
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  #endif
  
  I2C_Display->Print_Msg(ETH_IP_MSG);
  // give the Ethernet shield a second to initialize:
  delay(2000);
  //I2C_Display->Print_Msg(ETH_SERVER_CONNECTING_MSG);
  ETH_Status = STATUS_ETH_DISCONNECTED;
  Server_Connect();
  snprintf(Token, sizeof(Token),"token %s:%s",SDCard->Read_Key(),SDCard->Read_Secret());
  Serial.print("Token:");
  Serial.println(Token);
 /* Ulong_Test=123456789;
  PunchIn(Ulong_Test);
  PunchOut(Ulong_Test);*/
}
/*!
 * The function inform if there is new pendign data in the ETH connection
 * 
 * @param : - 
 * @return: True or False
*/
bool Eth_Client::Is_Incomming_Data()
{
  //return New_ETHData;
  if(client.available()) 
  {
    while(client.available())
    {
      char c = client.read();
      Serial.print(c);
    }
  }
  return client.available();
}
/*!
 * This function that connect to the server.
 * The status will be stored in ETH_Status variable
 * 
 * @param *Data: pointer do returned data
 * @return: TRUE or False if the data is valid
*/
void Eth_Client::Server_Connect()
{
  int Length;
  
  I2C_Display->Print_Msg(ETH_SERVER_CONNECTING_MSG);
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.println("connecting...");
  #endif
  if(client.connect(SDCard->Read_ServerName(), 80)) 
  {
    #ifdef ENABLE_SERIAL_DEBUG
        Serial.println("Server Connection Successful");
    #endif
    ETH_Status = STATUS_ETH_CONNECTED; 
    I2C_Display->Print_Msg(ETH_SERVER_CONNECTED_MSG);
  }
  else
  {
    ETH_Status = STATUS_ETH_DISCONNECTED; 
    I2C_Display->Print_Msg(ETH_SERVER_ERROR_MSG);
    #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("connection failed");
    #endif 
  }
  client.stop();
}
/*!
 * The function that inform the Ethernet Status using the flags described in
 * Hardware.h
 * 
 * @param : - 
 * @return: value of the Status variable ETH_Status
*/
char Eth_Client::Ethernet_Status()
{
  return ETH_Status;
}
/*!
 * This function return the configures IP on W5500
 * 
 * @param *Data: pointer do returned data
 * @return: TRUE or False if the data is valid
*/
IPAddress Eth_Client::LocalIP()
{
  return Ethernet.localIP();
}
/*!
 * The function that make the PunchIn Transaction.
 * The result is stored in the struct Punch
 * 
 * @param : - 
 * @return: Status of the transaction
*/
bool Eth_Client::PunchIn(unsigned long RFID_Tag)
{
  String Response;
  int Error;
  char *pChar_Start, *pChar_End;
     
  snprintf(Aux_String,sizeof(Aux_String),"/api/method/erpnext.hr.doctype.employee_checkin.employee_checkin.punch_in?rfid_tag=%lu",RFID_Tag);
//  Serial.println(Aux_String);
  client.beginRequest();
  Error = client.post(Aux_String);
  client.sendHeader("Authorization", Token);
  client.endRequest();

  if(Error == 0)
  {
    #ifdef ENABLE_SERIAL_DEBUG
     // Serial.println("startedRequest ok");
    #endif
    Error = client.responseStatusCode();
    if(Error >= 0)
    {
      #ifdef ENABLE_SERIAL_DEBUG
      //  Serial.print("Got status code: ");
     //   Serial.println(Error);
      #endif  
      delay(DELAY_HTTP);
     // Beep.Long_Beep();
      //  Is_Incomming_Data();
      Response = client.responseBody();
      #ifdef ENABLE_SERIAL_DEBUG
     //   Serial.println(Response);
      #endif   
      Response.toCharArray(Aux_String, BUFFER_LENGTH);
      Aux_String[BUFFER_LENGTH-1]=0;
      pChar_Start = strstr(Aux_String,KEY_STATUS);
      if(pChar_Start)
      {
        //************************ SUCCESS **********************************/
        if(strstr(pChar_Start,KEY_SUCCESS))   // Process the success message
        {
          /******************** in_time ************************/
          pChar_Start = strstr(pChar_Start,KEY_IN_TIME);
          pChar_Start = strstr(pChar_Start,":");                      // Move to the end " of In_Time
          pChar_Start+=2;
          pChar_End = strstr(pChar_Start,",");                        // Move to the end " of time
          pChar_End--;
          *pChar_End = 0;                                             // End String in the ',' char
          strcpy(Punch_Data.In_Time,pChar_Start);                     // Copy the In time
         /* #ifdef ENABLE_SERIAL_DEBUG
            Serial.print("IN_TIME = ");
            Serial.println(Punch_Data.In_Time);
          #endif */
          pChar_Start = pChar_End+1;                                  // Move the pointer to read the next field
          /******************** Employee ************************/
          pChar_Start = strstr(pChar_Start,":");                      // Move to the end " of Employee
          pChar_Start+=2;
          pChar_End = strstr(pChar_Start,"}");                        // Move to the end " of time
          pChar_End--;
          *pChar_End = 0;                                             // End String in the ',' char
          strcpy(Punch_Data.Employee_Name,pChar_Start);               // Copy the Employee
        /*  #ifdef ENABLE_SERIAL_DEBUG
            Serial.print("EMPLOYEE = ");
            Serial.println(Punch_Data.Employee_Name);
          #endif*/
          I2C_Display->Print_Msg(COME_VALID_DATA_MSG);                // Print the valid message in the LCD 
          Beep.Low_High_Beep();
        }
        else
        {
          //************************ ERROR **********************************/
          if(strstr(pChar_Start,KEY_ERROR))                      // Process the error message
          {
            pChar_Start = strstr(pChar_Start,KEY_ERROR_MSG);
            if(strstr(pChar_Start,KEY_RFID_ERROR))               // RFID Error  
            {
              #ifdef ENABLE_SERIAL_DEBUG
                Serial.println(KEY_RFID_ERROR);
              #endif 
              I2C_Display->Print_Msg(RFID_ERROR_TAG_MSG); 
            }
            if(strstr(pChar_Start,KEY_EMPL_ERROR))               // Employee not active
            {
              #ifdef ENABLE_SERIAL_DEBUG
                Serial.println(KEY_EMPL_ERROR);
              #endif 
              I2C_Display->Print_Msg(EMPLOYEE_ERROR_MSG); 
            }
            if(strstr(pChar_Start,KEY_ALREADY_PUNCHIN_ERROR))    // Already Punch in
            {
              #ifdef ENABLE_SERIAL_DEBUG
                Serial.println(KEY_ALREADY_PUNCHIN_ERROR);
              #endif 
              I2C_Display->Print_Msg(ALREADY_ERROR_MSG); 
            }
            Beep.High_Low_Beep();
          }
        }
      }
    }
    else
    {    
      ETH_Status = STATUS_ETH_DISCONNECTED; 
      I2C_Display->Print_Msg(ETH_SERVER_ERROR_MSG);
      #ifdef ENABLE_SERIAL_DEBUG
        Serial.println("connection failed");
      #endif 
    }
  }
  else
  {
    ETH_Status = STATUS_ETH_DISCONNECTED; 
    I2C_Display->Print_Msg(ETH_SERVER_ERROR_MSG);
    #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("connection failed");
    #endif 
  }
  client.stop();
}
/*!
 * The function that make the PunchOut Transaction.
 * The result is stored in the struct Punch
 * 
 * @param : - 
 * @return: Status of the transaction
*/
bool Eth_Client::PunchOut(unsigned long RFID_Tag)
{
  String Response;
  int Error;
  char *pChar_Start, *pChar_End;
     
  snprintf(Aux_String,sizeof(Aux_String),"/api/method/erpnext.hr.doctype.employee_checkin.employee_checkin.punch_out?rfid_tag=%lu",RFID_Tag);
 // Serial.println(Aux_String);
  client.beginRequest();
  Error = client.post(Aux_String);
  client.sendHeader("Authorization", Token);
  client.endRequest();

  if(Error == 0)
  {
    #ifdef ENABLE_SERIAL_DEBUG
  //    Serial.println("startedRequest ok");
    #endif
    Error = client.responseStatusCode();
    if(Error >= 0)
    {
      #ifdef ENABLE_SERIAL_DEBUG
    //    Serial.print("Got status code: ");
     //   Serial.println(Error);
      #endif  
      delay(DELAY_HTTP);
     // Beep.Long_Beep();
      //  Is_Incomming_Data();
      Response = client.responseBody();
      #ifdef ENABLE_SERIAL_DEBUG
    //    Serial.println(Response);
      #endif    
      Response.toCharArray(Aux_String, BUFFER_LENGTH);
      Aux_String[BUFFER_LENGTH-1]=0;
      pChar_Start = strstr(Aux_String,KEY_STATUS);
      if(pChar_Start)
      {
        //************************ SUCCESS **********************************/
        if(strstr(pChar_Start,KEY_SUCCESS))   // Process the success message
        {
          /******************** Out_time ************************/
          pChar_Start = strstr(pChar_Start,KEY_OUT_TIME);
          pChar_Start = strstr(pChar_Start,":");                      // Move to the end " of In_Time
          pChar_Start+=2;
          pChar_End = strstr(pChar_Start,",");                        // Move to the end " of time
          pChar_End--;
          *pChar_End = 0;                                             // End String in the ',' char
          strcpy(Punch_Data.Out_Time,pChar_Start);                     // Copy the In time
          #ifdef ENABLE_SERIAL_DEBUG
          //  Serial.print("OUT_TIME = ");
          //  Serial.println(Punch_Data.Out_Time);
          #endif 
          pChar_Start = pChar_End+1;                                  // Move the pointer to read the next field
          /******************** in_time ************************/
          pChar_Start = strstr(pChar_Start,KEY_IN_TIME);
          pChar_Start = strstr(pChar_Start,":");                      // Move to the end " of In_Time
          pChar_Start+=2;
          pChar_End = strstr(pChar_Start,",");                        // Move to the end " of time
          pChar_End--;
          *pChar_End = 0;                                             // End String in the ',' char
          strcpy(Punch_Data.In_Time,pChar_Start);                     // Copy the In time
          #ifdef ENABLE_SERIAL_DEBUG
          //  Serial.print("IN_TIME = ");
          //  Serial.println(Punch_Data.In_Time);
          #endif 
          pChar_Start = pChar_End+1;                                  // Move the pointer to read the next field
          /******************** Employee ************************/
          pChar_Start = strstr(pChar_Start,":");                      // Move to the end " of Employee
          pChar_Start+=2;
          pChar_End = strstr(pChar_Start,",");                        // Move to the end " of time
          pChar_End--;
          *pChar_End = 0;                                             // End String in the ',' char
          strcpy(Punch_Data.Employee_Name,pChar_Start);               // Copy the Employee
          #ifdef ENABLE_SERIAL_DEBUG
          //  Serial.print("EMPLOYEE = ");
          //  Serial.println(Punch_Data.Employee_Name);
          #endif 
          pChar_Start = pChar_End+1;                                  // Move the pointer to read the next field
          /******************** Duration ************************/
          pChar_Start = strstr(pChar_Start,KEY_DURATION);
          pChar_Start = strstr(pChar_Start,":");                      // Move to the end " of Duration
          pChar_Start+=2;
          pChar_End = strstr(pChar_Start,",");                        // Move to the end " of Duration
          pChar_End--;
          *pChar_End = 0;                                             // End String in the ',' char
          strcpy(Punch_Data.Duration,pChar_Start);                    // Copy the In time
          #ifdef ENABLE_SERIAL_DEBUG
          //  Serial.print("DURATION = ");
          //  Serial.println(Punch_Data.Duration);
          #endif 
          I2C_Display->Print_Msg(GO_VALID_DATA_MSG);                  // Print the valid message in the LCD 
          Beep.Low_High_Beep();
        }
        else
        {
          //************************ ERROR **********************************/
          if(strstr(pChar_Start,KEY_ERROR))                      // Process the error message
          {
            pChar_Start = strstr(pChar_Start,KEY_ERROR_MSG);
            if(strstr(pChar_Start,KEY_RFID_ERROR))               // RFID Error  
            {
              #ifdef ENABLE_SERIAL_DEBUG
                Serial.println(KEY_RFID_ERROR);
              #endif 
              I2C_Display->Print_Msg(RFID_ERROR_TAG_MSG); 
            }
            if(strstr(pChar_Start,KEY_EMPL_ERROR))               // Employee not active
            {
              #ifdef ENABLE_SERIAL_DEBUG
                Serial.println(KEY_EMPL_ERROR);
              #endif 
              I2C_Display->Print_Msg(EMPLOYEE_ERROR_MSG); 
            }
            if(strstr(pChar_Start,KEY_NO_PUNCHIN_ERROR))        // Not Punch in
            {
              #ifdef ENABLE_SERIAL_DEBUG
                Serial.println(KEY_NO_PUNCHIN_ERROR);
              #endif 
              I2C_Display->Print_Msg(NO_PUNCHIN_ERROR_MSG); 
            }
            Beep.High_Low_Beep();
          }
        }
      } 
    }
    else
    {    
      ETH_Status = STATUS_ETH_DISCONNECTED; 
      I2C_Display->Print_Msg(ETH_SERVER_ERROR_MSG);
      #ifdef ENABLE_SERIAL_DEBUG
        Serial.println("connection failed");
      #endif 
    }
  }
  else
  {
    ETH_Status = STATUS_ETH_DISCONNECTED; 
    I2C_Display->Print_Msg(ETH_SERVER_ERROR_MSG);
    #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("connection failed");
    #endif 
  }
  client.stop();
}
/*!
 * This function returns the pointer to char array
 * of the Input time
 * 
 * @param: -
 * @return: pointer to char array
*/
char* Eth_Client::Read_In_Time()
{
  return Punch_Data.In_Time;
}
/*!
 * This function returns the pointer to char array
 * of the Output time
 * 
 * @param: -
 * @return: pointer to char array
*/
char* Eth_Client::Read_Out_Time()
{
  return Punch_Data.Out_Time;
}
/*!
 * This function returns the pointer to char array
 * of the Employee Name
 * 
 * @param: -
 * @return: pointer to char array
*/
char* Eth_Client::Read_Employee_Name()
{
  return Punch_Data.Employee_Name;
}
/*!
 * This function returns the pointer to char array
 * of the Duration time
 * 
 * @param: -
 * @return: pointer to char array
*/
char* Eth_Client::Read_Duration()
{
  return Punch_Data.Duration;
}
