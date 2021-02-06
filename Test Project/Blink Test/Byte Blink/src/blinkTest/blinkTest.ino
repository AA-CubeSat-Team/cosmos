/* Blink Test: LED on for 250 ms, off for 250 ms, repeatedly.
 *  
 * Most Arduinos have an on-board LED you can control.
 * Uno & Leonardo: On-board LED is attached to digital pin 13.
 * 
 * If you're unsure what pin the on-board LED is connected to on 
 * your Arduino model, check the documentation at:
 * http://www.arduino.cc
 * 
 * This example code is in the public domain.
 * Modified May, 8, 2014
 * By Scott Fitzgeradl
 */

/* COSMOS must identify the tlm packet and our defined interface
 * uses length as the method to determine when packets are complete,
 * so our tlm needs to contain: 
 * 
 * id
 * length
 */
struct soh_t
{
  byte length;
  byte id;  
  byte delay;
};

// Global variable to hold State of Health tlm packet
soh_t mySoh;

// Runs steup() func. when press the reset or power the board
void setup() 
{
  mySoh.id = 1;
  mySoh.delay = 10;

  pinMode(13, OUTPUT);  // on-board LED pin number
  Serial.begin(9600);   // serial baud rate
}

void loop() 
{
  processCmds();
  
  digitalWrite(13,HIGH);  // turn the LED on
  delay(10*mySoh.delay);     // wait for [mySoh.delay] * 0.01 sec
  digitalWrite(13,LOW);   // turn the LED off
  delay(10*mySoh.delay);     // wait 
  
  sendTlm();
}

///////////////////////////////////////////////////////////////

// Define Functions
void processCmds()
{
  while(Serial.available())
  {
    // get the new byte:
    mySoh.delay = Serial.read();
  }
}

void sendTlm()
{
  mySoh.length = sizeof(mySoh);
  writeTlm((const char*)&mySoh, sizeof(mySoh));
}
  
void writeTlm(const char* pkt, byte size)
{
  for(int i=0; i<size; i++)
  {
    Serial.write(pkt[i]);
  }
}
