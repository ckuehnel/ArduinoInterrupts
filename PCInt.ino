// Title      : PCINT
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : PCInt.ino
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

const int pLED = 13;             // LED at Pin13
const int pPCINT8 = A0;          // PCINT8 at Analog In 0
const int pPCINT9 = A1;          // PCINT9 at Analog In 1

volatile boolean iflag = true;

int idx;

// Install the interrupt routine for PCINT
ISR(PCINT1_vect)       
{
  if ( !(PINC & (1<<PINC0)) ) // Detects Change at Analog In 0
  {
    iflag = false;
    digitalWrite(pLED, HIGH);
  }  
  if ( !(PINC & (1<<PINC1)) ) // Detects Change at Analog In 1
  {
    iflag = true;
    digitalWrite(pLED, LOW);
  }
}

void setup()
{
  Serial.begin(19200);
  pinMode(pLED, OUTPUT);
  digitalWrite(pLED, LOW);
  
  pinMode(pPCINT8, INPUT);
  digitalWrite(pPCINT8, HIGH); // Pullup active
  
  pinMode(pPCINT9, INPUT);
  digitalWrite(pPCINT9, HIGH); // Pullup active
  
  PCICR = 1<<PCIE1; // PCINT8 - PCINT15 enabled
  Serial.print("PCICR: "); Serial.println(PCICR, HEX);
  PCMSK1 |= (1<<PCINT8);    //  tell pin change mask to listen to Analog In 0
  Serial.print("PCMSK1: "); Serial.println(PCMSK1, HEX);
  PCMSK1 |= (1<<PCINT9);    //  tell pin change mask to listen to Analog In 0
  Serial.print("PCMSK1: "); Serial.println(PCMSK1, HEX);
  sei();
  Serial.println("Setup finished.");
}

void loop()
{ 
  if (iflag) Serial.println(idx);  // iflag controls serial output
  idx++;
  delay(500);
}

