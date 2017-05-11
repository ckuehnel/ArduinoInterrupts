// Title      : ADC3s
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : ADC3s.ino
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

#define ADC0 0
#define TEMP 0b1000
#define VBG  0b1110

const int pLED = 13;             // LED at Pin13
const float VACC = 5.12;         // measured on my Arduino

unsigned int reload = 0xFFFF - 0xF424; // TCNT1 Reload for one second@16 MHz clock & prescaler 256
volatile unsigned int ADC_result;
volatile boolean ADC_flag = false;
float voltage;

// Install the interrupt routine for Timer1 Overflow
ISR(TIMER1_OVF_vect)       
{
  TCNT1 = reload;
}

// Install the interrupt routine for ADC Interrupt 
ISR(ADC_vect)
{
  ADC_flag = true;
  flash();
}

void setup()
{
  Serial.begin(19200);
  pinMode(pLED, OUTPUT);
  digitalWrite(pLED, LOW);
  
  cli();
  TCCR1A = 0;
  TCCR1B = 0;                       // Stop Timer1
  TCNT1 = reload;
  TCCR1B = (1<<CS12);               // Normal Mode, Prescaler = 256
  TIMSK1 = (1<<TOIE1);              // Timer1 Overflow Interrupt enable
 
  ADMUX = (0<<REFS1) | (1<<REFS0);  // AVCC is reference
  ADMUX |= VBG;                     // VBG selected
  ADCSRA |= (1<<ADEN) | (1<<ADATE) | (1<<ADIE);   // ADC enabled, prescaler unchanged
  ADCSRB = (1<<ADTS2) | (1<< ADTS1);              // Triggered by Timer/Counter1 Overflow
  ADCSRA |= (1<<ADSC);
  sei();

  Serial.println("Timer1 Configuration");
  Serial.print("TCCR1A: "); Serial.println(TCCR1A, HEX); 
  Serial.print("TCCR1B: "); Serial.println(TCCR1B, HEX); 
  Serial.print("TIMSK1: "); Serial.println(TIMSK1, HEX);
  
  Serial.println("ADC Configuration for Timer triggered Mode");
  Serial.print("ADMUX: "); Serial.println(ADMUX, HEX); 
  Serial.print("ADCSRA: "); Serial.println(ADCSRA, HEX);
  Serial.print("ADCSRB: "); Serial.println(ADCSRB, HEX);
  Serial.print("DIDR0: "); Serial.println(DIDR0, HEX);
  Serial.println("Setup finished.");
  delay(1000);
  Serial.println("ADC Timer triggered...");
}

void loop()
{  
  if (ADC_flag)
  { 
    cli();
    ADC_result = ADC;
    sei();
    Serial.println();
    Serial.print("ADC: "); Serial.print(ADC_result,HEX);
    voltage = ADC_result* VACC/1024;
    Serial.print("\tVoltage: "); Serial.print(voltage,3); Serial.println(" V");
    ADC_flag = false;
  }
  else
  {
    Serial.print(".");
    delay(100);
  }
}

void flash() 
{
  static boolean output = HIGH;

  digitalWrite(pLED, output);
  output = !output;
}

