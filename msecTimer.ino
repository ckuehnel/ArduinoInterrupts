// Title      : msecTimer
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : msecTimer.ino
// Testes w/  : Arduino 1.8.0
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
volatile byte count;
byte reload = 0x9C; // reload value for 10 ms Timer2 interrupt

// Install the interrupt routine for Timer2 CompareA
ISR(TIMER2_COMPA_vect) 
{
  count++;
  OCR2A = reload;
}

void setup()
{
  Serial.begin(19200);
  pinMode(pLED, OUTPUT);
  digitalWrite(pLED, LOW);

  cli();
  TCCR0B = 0;   // stop timer0
  OCR2A = reload; 
  Serial.print("OCR2A: "); Serial.println(OCR2A, HEX);
  TCCR2A = 1<<WGM21;
  Serial.print("TCCR2A: "); Serial.println(TCCR2A, HEX); 
  TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20);
  Serial.print("TCCR2B: "); Serial.println(TCCR2B, HEX); 
  TIMSK2 = (1<<OCIE2A);
  Serial.print("TIMSK2: "); Serial.println(TIMSK2, HEX);
  sei(); 
  Serial.println("Setup finished.");
}

void loop()
{ 
  if (count == 50) 
  {  
    flash();
    Serial.print(".");
    count = 0;
  }
}

void flash() 
{
  static boolean output = HIGH;

  digitalWrite(pLED, output);
  output = !output;
}

