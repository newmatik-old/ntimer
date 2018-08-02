/*
 * Copyright 2018 ESO Electronic and Contributors
 * License: MIT License (see LICENSE file)
 */

// Libraries for LCD Display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Ethernet
#include <Ethernet2.h>

// Libraries required for RFID
#include <SPI.h>
#include <MFRC522.h>

// HTTP calls
#include <HttpClient.h>

// Pin definition for RFID reader
#define SS_PIN 9 // SDA on Pin 10 for Uno, 9 for Mega
#define RST_PIN 8 // RST on Pin 9 for Uno, 8 for Mega

// Buttons and their pins on the board
const int startButtonPin = 2;
const int stopButtonPin = 3;
const int buzzerPin = 4;

const String softwareVersion = "0.0.1";

// Ethernet Configuration
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
const byte ip[] = { 10, 0, 0, 177 };

// RFID and LCD Configuration
MFRC522 mfrc522(SS_PIN, RST_PIN); // Creating and naming RFID reader
LiquidCrystal_I2C lcd(0x27, 20, 4); // Address 0x27, 20 characters in 4 lines

// variables for reading the pushbutton status
int startButtonState = 0;
int stopButtonState = 0;
int timeout = 30;

// Constructor
void setup() 
{
  // initialize the pushbutton pins as an input, buzzer as output
  pinMode(startButtonPin, INPUT);
  pinMode(stopButtonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
  Serial.println("Opening serial connection for debugging monitor");
  Serial.println("Software version: " + softwareVersion);

  Serial.println("Initializing and clearing LCD display, turning backlight on");
  lcd.init();
  clearScreen();
  lcd.backlight();

  Serial.println("Turning on ethernet");
  Ethernet.begin(mac, ip); 
  Serial.println("Local IP is: " + Ethernet.localIP());

  Serial.println("Opening SPI and Initializing RFID reader");
  SPI.begin(); 
  mfrc522.PCD_Init();

  startupscreen();
  
  login();

  Serial.println("Initialization completed"); // debugging statement
}

void buzzTone(int frequency, int duration){
  tone(buzzerPin, frequency); // Send 1KHz sound signal...
  delay(duration);        // ...for 100 ms
  noTone(buzzerPin);
}

/*
 * Show Startup Screen
 */

void startupscreen(){
   // Initial startup bootscreen
  lcd.setCursor ( 0, 0 ); // go to the top left corner
  lcd.print("ESO Electronic"); // write this string on the top row
  lcd.setCursor ( 0, 1 ); // go to the 2nd row
  lcd.print("ERPNext Tag Reader "); // pad string with spaces for centering
  lcd.setCursor ( 0, 2 ); // go to the third row
  lcd.print("Version: " + softwareVersion); // pad with spaces for centering
  lcd.setCursor ( 0, 3 ); // go to the fourth row
  lcd.print("IP: " + Ethernet.localIP());
  delay(1000);
  
  clearScreen();
 }

bool login(){
  lcd.setCursor ( 0, 0 ); // go to the top left corner
  lcd.print("Logging into ERPNext"); // write this string on the top row
  lcd.setCursor ( 0, 1 ); // go to the 2nd row
  lcd.print("Server:             "); // pad string with spaces for centering
  lcd.setCursor ( 0, 2 ); // go to the third row
  lcd.print("erp.es-electronic.co" + softwareVersion); // pad with spaces for centering
  lcd.setCursor ( 0, 3 ); // go to the fourth row
  
  lcd.print("SUCCESSFULL         ");
  delay(2000);
  
  clearScreen();
  }

/**
 * Clear LCD Display
 */
 void clearScreen(){
  lcd.setCursor ( 0, 0 ); // go to the top left corner
  lcd.print("                    "); // write this string on the top row
  lcd.setCursor ( 0, 1 ); // go to the 2nd row
  lcd.print("                    "); // pad string with spaces for centering
  lcd.setCursor ( 0, 2 ); // go to the third row
  lcd.print("                    "); // pad with spaces for centering
  lcd.setCursor ( 0, 3 ); // go to the fourth row
  lcd.print("                    ");
 }

/**
 * get RFID card ID as Integer
 */
unsigned long getID(){
  unsigned long UID_unsigned;
  UID_unsigned =  mfrc522.uid.uidByte[0] << 24;
  UID_unsigned += mfrc522.uid.uidByte[1] << 16;
  UID_unsigned += mfrc522.uid.uidByte[2] <<  8;
  UID_unsigned += mfrc522.uid.uidByte[3];
  
  Serial.println();
  Serial.println("UID Unsigned int"); 
  Serial.println(UID_unsigned);

  lcd.setCursor ( 0, 3 ); 
  lcd.print("Chip read           ");
  delay(500);
  
  lcd.setCursor ( 0, 3 );
  lcd.print("                    ");
  delay(100);
  
  lcd.setCursor ( 0, 3 );
  lcd.print(UID_unsigned);
  delay(1000);

  lcd.setCursor ( 0, 3 );
  lcd.print("                    ");
  return UID_unsigned;
}

void checkForRead(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) // If no RFID card recognized
  {
    return; // jump back and continue loop
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) // Wenn if no RFID senderwas selected
  {
    return; // jump back and continue loop
  }
  buzzTone(3000, 300);
  getID();
  timeout = 0;
}

void loop() // This is Adruino's loop function
{
  timeout = 30;
  startButtonState = digitalRead(startButtonPin);
  stopButtonState = digitalRead(stopButtonPin);
  
  lcd.setCursor (0, 0); 
  lcd.print("WAITING             ");
  lcd.setCursor (0, 1);
  lcd.print("Press Start or Stop ");
  

  if (startButtonState == HIGH) {
    Serial.println("Start Button Pressed");
    buzzTone(2000, 100);
    
    lcd.setCursor (0, 0); 
    lcd.print("STARTING WORK       ");
    lcd.setCursor (0, 1); 
    lcd.print("Present RFID Chip   ");
    
    while(timeout > 0){
      checkForRead();
      delay(100);
      timeout = timeout -1;
    }
    return;
  }
  
  if (stopButtonState == HIGH) {
    Serial.println("Stop Button Pressed");
    buzzTone(2000, 100);
    
    lcd.setCursor (0, 0); 
    lcd.print("STOPPING WORK       ");
    lcd.setCursor (0, 1); 
    lcd.print("Present RFID Chip   ");

    while(timeout > 0){
      checkForRead();
      delay(100);
      timeout = timeout -1;
    }
    return;
  }
}
