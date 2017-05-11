// Title      : ACOMP
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : acomp.ino
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
const int pAIN1 = 7;             // AIN1 at Pin7

volatile boolean iflag = true;

int idx;

// Install the interrupt routine for ACOMP
ISR(ANALOG_COMP_vect)       
{
  if ( ACSR & (1<<ACO) )       // ACO is set?
  {
    iflag = false;
    digitalWrite(pLED, HIGH);
  }  
  else
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
  
  pinMode(pAIN1, INPUT);
  
  cli();
  ADCSRA &= ~(1<<ADEN);                // ADC disabled
  ADCSRB |= ~(1<<ACME);                // AMUX enabled
  ACSR = (1<<ACBG) | (1<<ACIE);        // ACOMP Interrupt enabled
  DIDR1 = (1<<AIN1D) | (1<< AIN0D);        
  sei();
  
  Serial.print("ADCSRA: "); Serial.println(ADCSRA, HEX);                 
  Serial.print("ADCSRB: "); Serial.println(ADCSRB, HEX);  
  Serial.print("ACSR: "); Serial.println(ACSR, HEX);
  Serial.print("DIDR1: "); Serial.println(DIDR1, HEX);
  Serial.println("Setup finished.");
}

void loop()
{ 
  if (iflag) Serial.println(idx);  // iflag controls serial output
  idx++;
  delay(500);
}

