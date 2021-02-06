/* Photocell simple testing sketch. 
 
Connect one end of the photocell to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground 
Connect LED from pin 11 through a resistor to ground 
For more information see http://learn.adafruit.com/photocells 
Code source: https://learn.adafruit.com/photocells/using-a-photocell?view=all
*/
/* Blink Test: LED on/off periodically.
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
////////////////////////////////////////////////////////////////////////////////
/* Telemetry configuration structure (Seperate the structure by data type)
 *  length, id, delay, ir_value, mult, led_delay
 */
struct ir_soh
{
  byte length;		// Packet ID so COSMOS can identify the packet and store it.
  byte id;			// Packet ID so COSMOS can identify the packet and store it.
  int mult_delay;	// Delay between LED blinks = MultDelay * 0.1 sec
  int ir_value;		// Photocell reading (analog value).
};

ir_soh mySoh;			// Global variable to hold State of Health tlm packet

int photocellPin = A0;	// the cell and 10K pulldown are connected to a0
unsigned int photocellReading;	// the analog reading from the sensor divider
int LEDpin = 11;		// connect Red LED to pin 11 (PWM pin)
unsigned int LEDbrightness;		//
unsigned int MultDelay;			// Delay between LED blinks = MultDelay * 0.1 sec
int n;
////////////////////////////////////////////////////////////////////////////////

void setup(void) 
{
  /* Initialize the variables 
	(Once the board is reset, they will always be these values) */
  mySoh.id = 1;
  MultDelay = 100;
//  uint2byteREV(MultDelay, mySoh.mult_delay);
  mySoh.mult_delay = MultDelay;
  
  /* Assign pinMode*/
  pinMode(13, OUTPUT);          // [!] Don't FORGOT the OUTPUT Pin
  pinMode(photocellPin, INPUT); // [!] Don't FORGOT the INPUT Pin
  
  /* Assign transmission rate for Serial port connection*/
  Serial.begin(9600);   		//transferring a maximum of 9600 bits/sec
}
 
void loop(void) 
{
  processCmds();			// Receive commands from the COSMOS

  digitalWrite(13,HIGH);	// LED on
  n = MultDelay;
  for (int i = 1; i <= n; i++)
  {	
	  getIR();				// Get IR sensor analog reading
	  delay(100);			// Sensor delay 0.1 sec
	  sendTlm();			// Send data to COSMOS telemetry
  }
  
  digitalWrite(13,LOW);		// LED off
  for (int j = 1; j <= n; j++)
  {	
	  getIR();				// Get IR sensor analog reading
	  delay(100);			// Sensor delay 0.1 sec
	  sendTlm();			// Send data to COSMOS telemetry
  }
}

////////////////////////////////////////////////////////////////////////////////
/* Get IR sensor analog reading */
void getIR()
{    
  /* Gathering sensor data */
  photocellReading = analogRead(photocellPin);	// analog IR reading
//  uint2byteREV(photocellReading, mySoh.ir_value);	// IR reading converts to 2-bytes array and store in mySoh
  mySoh.ir_value = photocellReading;
  
  /* Print IR value on the Arduino monitor (Will confilct with COSMOS. Comment off this when using COSMOS)*/
/*
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);     // the raw analog reading
*/
}

/* Convert int data type to a 2-bytes(16 bits) array*/
void uint2byteREV(unsigned int data, byte buf[])
{
  // Byte Array Buffer
  buf[1] = (data >> 0)  & 255;
  buf[0] = (data >> 8)  & 255;  // right shift 8 bits 
}

////////////////////////////////////////////////////////////////////////////////
/* cmd, tlm configuration with COSMOS */
/* Process commands from the COSMOS.
   Here we have is "DELAY_NUM" command
*/
void processCmds()
{
  while(Serial.available())
  {
    // get the new byte value for the "DELAY_NUM" command in COSMOS
    MultDelay = Serial.read();				// Reading in commands (int data type)
    mySoh.mult_delay = MultDelay;
  }
}

/* Send data from Arduino to COSMOS telemetry. */
void sendTlm()
{
  mySoh.length = sizeof(mySoh);
  // ------------------------- [Method 1] -------------------------
  //writeTlm((const char*)&mySoh, sizeof(mySoh));
  // ------------------------- [Method 2] -------------------------
  Serial.write(mySoh.length);		/* Serial Write */
  Serial.write(mySoh.id);
  Serial.write(mySoh.mult_delay,2);
  Serial.write(mySoh.ir_value,2);
}

/* Write the telemetry data */
void writeTlm(const char* pkt, byte size)
{
  for(int i=0; i<size; i++)
  {
    Serial.write(pkt[i]);
  }
}