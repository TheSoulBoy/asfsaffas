#include <Wire.h>


void setup()
{
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}


void loop()
{
}


void receiveEvent(int bytes)
{
  uint x = Wire.read();    // read one character from the I2C
  // prints current mode to console
  Serial.print("Slave Mode: ");
  Serial.println(x);
}
