/**********************************************************************
* ) 2018 ESO Electronic / Newmatik
*
; * FileName:        Hardware.h
; * Dependencies:    Header (.inc) files if applicable, see below
; * Processor:       ATMEGA2560
; * Compiler:        Arduino IDE
; *
 * REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author           Date      Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Juan Manuel Garin      08/08/18      First release of source file
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*  Code Description:
*  This file include all the pin declarations used by the hardware in Newmatik Ntimer v1.0
 *************************************************************************/
#ifndef Hardware_h
#define Hardware_h

#define BEEPER_PIN        11
#define COME_BUTTON_PIN   19
#define GO_BUTTON_PIN     18
#define RST_DISPLAY       42
#define BACKLIGHT_DISPLAY 12
#define RFID_RST_PIN      48    // Is not used
#define RFID_SS_PIN       5
#define RFID_IRQ_PIN      2
#define ETH_SS_PIN        43    // Used in Ethernet2.h
#define ETH_IRQ_PIN       2
#define ETH_RST_PIN       44
#define ETH_PMODE0_PIN    45
#define ETH_PMODE1_PIN    46
#define ETH_PMODE2_PIN    47
#define SD_SS_PIN         4

/*************** Library and Hardware Status flags *****************/
#define STATUS_FLAG_OK           0
#define STATUS_FLAG_ERROR        1
#define STATUS_FLAG_FAILED       2
#define STATUS_ETH_DISCONNECTED  3
#define STATUS_ETH_CONNECTED     4
#define STATUS_ETH_NO_IP         5
#define STATUS_SD_FAILS          6
#define STATUS_SD_NO_FILE        7

#define INITIAL_BACKLIGHT_BRIGHTNESS  255
#define NORMAL_BACKLIGHT_BRIGHTNESS   100
#define BEEPER_FREQUENCY        2730
#define HIGH_BEEPER_FREQUENCY   3500
#define LOW_BEEPER_FREQUENCY    1800
#define SHORT_BEEP_TIME         200     //200ms
#define LONG_BEEP_TIME          250     //250ms

#define ENABLE_SERIAL_DEBUG     true
#define STRING_LENGTH           50
#define COUNTDOWN_SEC           5
#define COUNT_VALID_SEC         1
#endif
