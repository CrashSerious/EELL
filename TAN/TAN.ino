
// I2C Slave Sender Template
// Template Arduino Node

#include <Wire.h>

void setup()
{
  Wire.begin(10);                // join i2c bus with address #10
  Wire.onRequest(requestEvent); // register event
   Serial.begin(9600);  // start serial for output
}

void loop()
{
  delay(100);
  //Serial.println ("hello22");
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
  Wire.write("hello "); // respond with message of 6 bytes
                       // as expected by master
                       
  Serial.println ("hello there");
  
  delay(1500);
}

