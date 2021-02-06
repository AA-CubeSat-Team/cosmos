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

#define PAYLOAD_BUF 64

// Arduino I/O Pin
#define LED_PIN 13			// pin the LED is connected to

// Default telemetry parameter
#define DEFAULT_DELAY 500	// blink delay (milisecond)

// Command ID
#define INCOMING_ID 0x00	// Incoming command id from COSMOS.

/* Tlemetry Packet: COSMOS must identify the tlm packet 
 * and our defined interface uses "length" as the method
 * to determine when packets are complete.
 * So our tlm needs to contain: 
 * 
 * id, length
 */
struct soh_t
{
  uint8_t length;
  uint8_t id;  
  uint16_t delay;
};

/* Command Packet */
struct blink_cmd
{
  uint8_t id;
  uint16_t delay;
}

/* Global packet */
soh_t mySoh;			// telemetry packet
blink_cmd cmd_struct;	// command packet

// Runs steup() func. when press the reset or power the board
void setup() 
{
  mySoh.id = 1;
  mySoh.delay = DEFAULT_DELAY;	// default delay = 500 ms = 0.5 sec

  pinMode(LED_PIN, OUTPUT);  // on-board LED pin number
  Serial.begin(9600);   // serial baud rate
}

void loop() 
{
  sendTlm();	// Sending and writing data to COSMOS
  blink();		// called blink() function for LED blink
  processCmds();// Read incoming command
}

///////////////////////////////////////////////////////////////

// Define Functions
// ----- LED Blink function -----
void blink()
{
  digitalWrite(LED_PIN,HIGH);	// turn the LED on
  delay(mySoh.delay);			// wait
  digitalWrite(LED_PIN,LOW);		// turn the LED off
  delay(mySoh.delay);			// wait 
}

// ----- Processing and reading the Commands from COSMOS -----
// -----  This will be called once each time through the loop() function
void processCmds()
{
  //Serial.println("Please input the delay time [ms]...");
  while(Serial.available())	// If data is available
  {
  // ------------------------- [Method 1] -------------------------
	mySoh.delay  = Serial.read();		// read it and store it
  // ------------------------- [Method 2] -------------------------
	//mySoh.delay  = Serial.readBytes(buf, BUFFER_SIZE);
  // ------------------------- [Method 3] -------------------------
    //mySoh.delay  = Serial.parseInt();
  }
  //Serial.print("The time delay is [ ");
  //Serial.print(mySoh.delay);
  //Serial.println(" ms]");
}

// ----- Sending and writing data to COSMOS --------------------
void sendTlm()
{
  mySoh.length = sizeof(mySoh);
  
  // ------------------------- [Method 1] -------------------------
  writeTlm((const char*)&mySoh, sizeof(mySoh));
  // ------------------------- [Method 2] -------------------------
  //Serial.write(mySoh.length);
  //Serial.write(mySoh.id);
  //sendBinary(mySoh.delay);
}
  
void writeTlm(const char *structPointer, byte structLength)
{
  for(int i=0; i<structLength; i++)
  {
    Serial.write(structPointer[i]);
  }
}

// ----- Function to send the given integer value to the serial port -----
void sendBinary(int value)
{
	// send the 2 bytes that compose a 2-byte (16 bits) integer
	Serial.write(lowByte(value));	// send the low byte
	Serial.write(highByte(value));	// send the high byte
}
	