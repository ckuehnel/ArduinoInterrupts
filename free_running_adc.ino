// Title      : Free Running ADC
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : free_running_adc.ino
// Tested w/  : Arduino 1.8.0
//
// DISCLAIMER:
// The author is in no way responsible for any problems or damage caused by
// using this code. Use at your own risk.
//
// LICENSE:
// This code is distributed under the GNU Public License
// which can be found at http://www.gnu.org/licenses/gpl.txt

#define ADC4 0x44
#define ADC5 0x45
#define TEMPERATURE 0x48
#define BANDGAP 0x4E
#define GND 0x4F

// ADCSRA
#define FREE_RUNNING_MODE ((1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0))

#define PURPOSE "Test of Free Running ADC"

const byte LED = 13;

word adc_value;

void setADC(byte config)
{
  ADCSRA &= ~(1<<ADEN); // ADC disabled
  ADMUX = config;       // MUX selection
  ADCSRA = FREE_RUNNING_MODE; // start ADC  
}

word getADC(void)
{  
  return (ADCL | (ADCH << 8));
}

void printResult(void)
{
  digitalWrite(LED, 1); // LED on
  delay(10);
  adc_value = getADC();
  digitalWrite(LED, 0); // LED off
  Serial.print(adc_value, HEX); // output ADC value
  Serial.print("\t");
  Serial.print((float) adc_value*5./1024); // calculate voltage and output
  Serial.println(" V\n");
}

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(19200);
  Serial.println(PURPOSE);
}

void loop()
{ 
  Serial.println("Bandgap Reference");
  setADC(BANDGAP); 
  printResult();
  delay(1000);
  Serial.println("GND");
  setADC(GND);
  printResult();
  delay(1000);
  Serial.println("Temperature");
  setADC(TEMPERATURE);
  printResult();
  delay(1000);
}

