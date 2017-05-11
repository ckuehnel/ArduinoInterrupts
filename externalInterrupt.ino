// Title      : External Interrupt
// Author     : Claus Kuehnel <ckuehnel@gmx.ch>
// Date       : 2017-05-10
// Id         : externalInterrupt.ino
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

const int pLED = 13; // LED at Pin13
const int pINT0 = 2; // INT0 at Pin2
const int pINT1 = 3; // INT1 at Pin3

volatile boolean iflag = true;

int idx;

void setup()
{
  Serial.begin(19200);
  pinMode(pLED, OUTPUT);
  
  pinMode(pINT0, INPUT);
  digitalWrite(pINT0, HIGH); // Pullup active
  
  pinMode(pINT1, INPUT);
  digitalWrite(pINT1, HIGH); // Pullup active
  
  attachInterrupt(0, stop_serial, FALLING); // INT0 stops serial output
  Serial.print("EICRA: "); Serial.println(EICRA, HEX);
  attachInterrupt(1, resume_serial, FALLING); // INT1 resumes serial output
  Serial.print("EICRA: "); Serial.println(EICRA, HEX);
  Serial.println("Setup finished.");
}

void loop()
{ 
  if (iflag) Serial.println(idx);  // iflag controls serial output
  idx++;
  delay(500);
}

void stop_serial()           
{
  iflag = false;
  digitalWrite(pLED, HIGH);
}

void resume_serial()
{
  iflag = true;
  digitalWrite(pLED, LOW);
}

