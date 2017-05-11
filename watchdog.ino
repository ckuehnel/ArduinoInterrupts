//
// Title      : Watchdog
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : watchDog.ino
// Tested w/  : Arduino 1.8.0
//
// DISCLAIMER:
// The author is in no way responsible for any problems or damage caused by
// using this code. Use at your own risk.
//
// LICENSE:
// This code is distributed under the GNU Public License
// which can be found at http://www.gnu.org/licenses/gpl.txt
//
// Definition of interrupt names
#include <avr/io.h>
// ISR interrupt service routine
#include <avr/interrupt.h>

#define  wdt_reset()   __asm__ __volatile__ ("wdr") 

const int pLED = 13; // LED at Pin13

int idx;

// Install the interrupt routine for Watchdog Interrupt
ISR(WDT_vect)       
{
  flash();
}

void setup()
{
  Serial.begin(19200);
  pinMode(pLED, OUTPUT);
  digitalWrite(pLED, LOW);
  
  cli();
  wdt_reset();
  WDTCSR |= (1<<WDCE) | (1<<WDE); // Start timed sequence
  WDTCSR = (1<<WDIE) | (1<<WDP2) | (1<<WDP1); // Set new prescaler = 128K cycles (~1 s)
  sei();
  
  Serial.print("WDTCSR: "); 
  Serial.println(WDTCSR, HEX);
  Serial.println("Setup finished.");
}

void loop()
{ 
  Serial.println(idx++); // do anything
  delay(1500); // change argument to 1500 -> watchdog will be active
  wdt_reset();
}

void flash() 
{
  static boolean output = HIGH;

  digitalWrite(pLED, output);
  output = !output;
}

