// Title      : ADC2s
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : ADC2s.ino
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

const int pLED = 13;            // LED at Pin13
const float VACC = 5.12;        // measured on my Arduino

volatile unsigned int ADC_result;
float voltage;

ISR(ADC_vect)
{
  ADC_result = ADC;
}

void setup()
{
  Serial.begin(19200);
  pinMode(pLED, OUTPUT);
  digitalWrite(pLED, LOW);
  
  cli();
  ADMUX = (0<<REFS1) | (1<<REFS0);     // AVCC is reference
  ADMUX |= VBG;                        // VBG selected
  ADCSRA |= (1<<ADEN) | (1<<ADATE) | (1<<ADIE);    // ADC enabled, prescaler unchanged
  ADCSRB = 0;                          // Free Running Mode
  ADCSRA |= (1<<ADSC);
  sei();

  Serial.println("ADC Configuration for Free Running Mode");
  Serial.print("ADMUX: "); Serial.println(ADMUX, HEX); 
  Serial.print("ADCSRA: "); Serial.println(ADCSRA, HEX);
  Serial.print("ADCSRB: "); Serial.println(ADCSRB, HEX);
  Serial.print("DIDR0: "); Serial.println(DIDR0, HEX);
  delay(1000);
  Serial.println("ADC free running...");
}

void loop()
{   
  cli();
  ADC_result = ADC;
  sei();
  Serial.print("ADC: "); Serial.print(ADC_result,HEX);
  voltage = ADC_result* VACC/1024;
  Serial.print("\tVoltage: "); Serial.print(voltage,3); 
  Serial.println(" V");
  delay(1000);
}

