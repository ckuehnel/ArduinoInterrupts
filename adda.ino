// Title      : ADDA
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : adda.ino
// Tested w/  : Arduino 1.8.0
//
// DISCLAIMER:
// The author is in no way responsible for any problems or damage caused by
// using this code. Use at your own risk.
//
// LICENSE:
// This code is distributed under the GNU Public License
// which can be found at http://www.gnu.org/licenses/gpl.txt

const int AOUT = 9; // IO9 is PWM output
const int AIN0 = 0; // AIN0 is analog input

void setup()
{
  pinMode(AOUT, OUTPUT); // configure PWM output
  Serial.begin(19200); // baud rate of console output
  Serial.println("DAC\tADC\tADC-4DAC");  
}

void loop()
{
  int i, val;
  for (i=0; i<256; i+=16)
  {
    analogWrite(AOUT, i); // set PWM value from 0 to 255
    delay(1000);
    Serial.print(i); // console output of DAC (PWM) value
    Serial.print("\t");
    val = analogRead(AIN0); // read ADC result 
    Serial.print(val); // console output of ADC result
    Serial.print("\t");
    Serial.println(val - 4*i); // calculate deviation and output it 
  }  
}

