/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        SD_Card.cpp
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
*  This object library manage the micro SD card
 *************************************************************************/

#include "SD_Card.h"

//************************ Globals *************************//
extern Display *I2C_Display;
const String FileName = "NTIMER.TXT";
const String Default_Server = "staging.newmatik.com";
const String Default_Secret = "909c8df0e367b13";
const String Default_Key = "f24f86282a17c86";
char Buffer[BUFFER_LENGTH];

/*!
 * Object constructor:
 *					This function initialize the pins and configure
 *					the SD library
 * @param : - 
 * @return: -
*/
SD_Card::SD_Card()
{
  #ifdef ENABLE_SERIAL_DEBUG
    Serial.print("Initializing SD card...");
  #endif
  pinMode(SD_SS_PIN, OUTPUT);                             // Setup the CS Pin
  digitalWrite(SD_SS_PIN, HIGH); 

  if(SD.begin(SD_SS_PIN))                                 // Try to open the SD Card
  {
    #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("initialization done");
    #endif
   
    MyFile = SD.open(FileName, O_READ);                      // Open file for reading   
    if(MyFile)
    {
      #ifdef ENABLE_SERIAL_DEBUG
        Serial.println(FileName + ":");
      #endif
      /***** Read the information from the configs File *******/
      Read_Config_File();
      
      MyFile.close();                               // Close the file
      SD_Status_Var = STATUS_FLAG_OK;               // Procedure successful
    }
    else 
    {
      #ifdef ENABLE_SERIAL_DEBUG
        Serial.print("error opening ");
        Serial.println(FileName);
      #endif
      SD_Status_Var = STATUS_SD_NO_FILE;              // ERROR no file
      I2C_Display->Print_Msg(SD_NO_FILE_MSG);  
    }
  }
  else
  {
    #ifdef ENABLE_SERIAL_DEBUG
      Serial.println("initialization failed!");
    #endif
    SD_Status_Var = STATUS_SD_FAILS;                  // SD ERROR
    Default_Server.toCharArray(Readed_Data.ServerName, STRING_LENGTH);
    Readed_Data.Dhcp = true;
    Default_Secret.toCharArray(Readed_Data.Secret, STRING_LENGTH);
    Default_Key.toCharArray(Readed_Data.Key, STRING_LENGTH);
    I2C_Display->Print_Msg(SD_FAILS_MSG);
  }   
}
/*!
 * The function that return if is enabled the DHCP
 * 
 * @param : - 
 * @return: True or False
*/
bool SD_Card::Is_DHCP_Enabled()
{
  return Readed_Data.Dhcp;
}
/*!
 * The function that return the Ip Address in IPAddress
 * format
 * 
 * @param : - 
 * @return: IP Address
*/
IPAddress SD_Card::Ip_Address()
{
  return Readed_Data.Ip;  
}
/*!
 * The function that return the Dns Address in IPAddress
 * format
 * 
 * @param : - 
 * @return: DNS Address
*/
IPAddress SD_Card::Dns_Address()
{
  return Readed_Data.Dns;  
}
/*!
 * The function that return the Gateway Address in IPAddress
 * format
 * 
 * @param : - 
 * @return: Gateway Address
*/
IPAddress SD_Card::Gateway_Address()
{
  return Readed_Data.Gateway;  
}
/*!
 * The function that returns the ServerName value
 * 
 * @param : - 
 * @return: string with the Server Name
*/
char* SD_Card::Read_ServerName()
{
  return Readed_Data.ServerName;
}
/*!
 * The function that return the Secret value
 * 
 * @param : - 
 * @return: string with the Secret phrase
*/
char* SD_Card::Read_Secret()
{
  return Readed_Data.Secret;
}
/*!
 * The function that return the Key value
 * 
 * @param : - 
 * @return: string with the Key
*/
char* SD_Card::Read_Key()
{
  return Readed_Data.Key;
}
/*!
 * The function that return if is enabled the Online Update
 * 
 * @param : - 
 * @return: True or False
*/
bool SD_Card::Is_OnlineUpdate_Enabled()
{
  return Readed_Data.OnlineUpdate;
}
/*!
 * The function that return if is enabled the SD Update
 * 
 * @param : - 
 * @return: True or False
*/
bool SD_Card::Is_SD_Update_Enabled()
{
  return Readed_Data.SD_Update;
}
/*!
 * The function that return the status flag of SD object
 * 
 * @param : - 
 * @return: Status Flags declared in Hardware.h
*/
char SD_Card::SD_Status()
{
  return SD_Status_Var;
}
/*!
 * The function that read char by char a string line from a file an put
 * in the Data passed as parameter.
 * When the char readed is the CRLF the functions finish.
 * If there are more chars in buffer it return TRUE, FALSE in another case
 * 
 * @param : - 
 * @return: Status Flags declared in Hardware.h
*/
bool SD_Card::Read_Line(char *Data)
{
  bool End_Line = false;
  char Char_Pos = 0;
  char line[50];
  while(MyFile.available() && !End_Line)                         // read from the file until there's nothing else in it:
  {
    Data[Char_Pos]=MyFile.read();     // Load the data in the strin variable
    if(Data[Char_Pos]=='\r')            // If the readed char is the end of the line
    {
       End_Line = true;
       Data[Char_Pos] = 0;            // End of string
    }
    else
    {
       if(Char_Pos<(BUFFER_LENGTH-2))
       {
        Char_Pos++;
       }
       if(Char_Pos==(BUFFER_LENGTH-1))
       {
        End_Line = true;
        Data[Char_Pos] = 0;            // End of string
        return false;
       }
    }
  }
  if(MyFile.available())
  {
    return true;
  }
  else
  {
    return false;
  }
}
/*!
 * The function that read all the file and processing the collected data
 * finding each value.
 * 
 * @param : - 
 * @return: Status Flags declared in Hardware.h
*/
void SD_Card::Read_Config_File()
{
  char Ip_Buffer[4];
  bool End_Of_File = false;
  char *pChar;
  while(!End_Of_File)
  {
    if(Read_Line(Buffer))
    {
      /******************** DHCP ************************/
      if(strstr(Buffer, "DHCP = "))               
      {
        if(Buffer[8]=='1')
        {
          Readed_Data.Dhcp = true;
          #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("DCHP Enabled");
          #endif 
        }
        else
        {
          Readed_Data.Dhcp = false;  
          #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("DCHP Disabled");
          #endif
        }   
      }
      /*********************** IP *************************/
      if(strstr(Buffer, "IP = "))               
      {
        for(char i=0;i<4;i++)
        {
          strncpy(Ip_Buffer,&Buffer[6+i*4], 3);
          Ip_Buffer[3]=0;
          Readed_Data.Ip[i]=atoi(Ip_Buffer); 
        }
        #ifdef ENABLE_SERIAL_DEBUG
          Serial.print("IP = ");
          Serial.println(Readed_Data.Ip);
        #endif 
      }
      /*********************** DNS *************************/
      if(strstr(Buffer, "DNS = "))               
      {
        for(char i=0;i<4;i++)
        {
          strncpy(Ip_Buffer,&Buffer[8+i*4], 3);
          Ip_Buffer[3]=0;
          Readed_Data.Dns[i]=atoi(Ip_Buffer); 
        }
        #ifdef ENABLE_SERIAL_DEBUG
          Serial.print("DNS = ");
          Serial.println(Readed_Data.Dns);
        #endif 
      }
      /******************** GATEWAY ***********************/
      if(strstr(Buffer, "GATEWAY = "))               
      {
        for(char i=0;i<4;i++)
        {
          strncpy(Ip_Buffer,&Buffer[12+i*4], 3);
          Ip_Buffer[3]=0;
          Readed_Data.Gateway[i]=atoi(Ip_Buffer); 
        }
        #ifdef ENABLE_SERIAL_DEBUG
          Serial.print("GATEWAY = ");
          Serial.println(Readed_Data.Gateway);
        #endif 
      }
      /******************** Server ************************/
      if(strstr(Buffer, "SERVER = "))               
      {
        strncpy(Readed_Data.ServerName,&Buffer[11],STRING_LENGTH);
        #ifdef ENABLE_SERIAL_DEBUG
          Serial.print("SERVER = ");
          Serial.println(Readed_Data.ServerName);
        #endif 
      }
      /******************** Secret ************************/
      if(strstr(Buffer, "SECRET = "))               
      {
        strncpy(Readed_Data.Secret,&Buffer[10],STRING_LENGTH);
        #ifdef ENABLE_SERIAL_DEBUG
          Serial.print("SECRET = ");
          Serial.println(Readed_Data.Secret);
        #endif 
      }
      /******************** Key ************************/
      if(strstr(Buffer, "KEY = "))               
      {
        strncpy(Readed_Data.Key,&Buffer[8],STRING_LENGTH);
        #ifdef ENABLE_SERIAL_DEBUG
          Serial.print("KEY = ");
          Serial.println(Readed_Data.Key);
        #endif 
      }
      /***************** Online Update *********************/
      if(strstr(Buffer, "ONLINE_UPDATE = "))               
      {
        if(Buffer[18]=='1')
        {
          Readed_Data.OnlineUpdate = true;
          #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("ONLINE_UPDATE Enabled");
          #endif 
        }
        else
        {
          Readed_Data.OnlineUpdate = false;  
          #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("ONLINE_UPDATE Disabled");
          #endif
        }
      }   
      /********************* SD Update *********************/
      if(strstr(Buffer, "SD_UPDATE = "))               
      {
        if(Buffer[14]=='1')
        {
          Readed_Data.SD_Update = true;
          #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("SD_UPDATE Enabled");
          #endif 
        }
        else
        {
          Readed_Data.SD_Update = false;  
          #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("SD_UPDATE Disabled");
          #endif
        }
      }
    }
    else
    {
      End_Of_File = true;
    }
  }
}
