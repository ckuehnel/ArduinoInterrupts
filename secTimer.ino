//
// Title      : secTimer
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : secTimer.ino
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
unsigned int reload = 0xF424;    // OCR1A Reload for one second@16 MHz clock & prescaler 256
volatile int count;

// Install the interrupt routine for Timer1 CompareA
ISR(TIMER1_COMPA_vect)       
{
  count++;
  flash();
}

void setup()
{
  Serial.begin(19200);
  pinMode(pLED, OUTPUT);
  digitalWrite(pLED, LOW);
  
  cli();
  TCCR1A = 0;
  TCCR1B = 0;                        // Stop Timer1
  OCR1A = reload;
  TCCR1B = (1<<WGM12) | (1<<CS12);   // CTC Mode Prescaler = 256
  TIMSK1 = (1<<OCIE1A);       // Timer1 CompareA Interrupt enable
  sei();                             // Global Interrupt enable
  
  Serial.print("OCR1A: "); Serial.println(OCR1A, HEX);
  Serial.print("TCCR1A: "); Serial.println(TCCR1A, HEX); 
  Serial.print("TCCR1B: "); Serial.println(TCCR1B, HEX); 
  Serial.print("TIMSK1: "); Serial.println(TIMSK1, HEX);
  Serial.println("Setup finished.");
}

void loop()
{ 
  Serial.println(count);              // do anything
  delay(200);
}

void flash() 
{
  static boolean output = HIGH;

  digitalWrite(pLED, output);
  output = !output;
}

