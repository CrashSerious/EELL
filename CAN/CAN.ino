// I2C Master Reader
// Central Arduino Node for EELL.

#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// variables will change:
int buttonState = 0;         // variable for reading the push button status
int requestnum;
const int buttonPin = 8;     // the number of the pushbutton pin
int SlaveAddress;
int MaxAddress = 100;
int CurrentAddress = 0; //set to the FIRST item

int SlaveDevices[100];
  
void setup()
{
  //init and declare variables
  char x;
  requestnum =0;
  
  //setup libraries
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  lcd.begin(16, 2);

  //setup inputs and outputs
  pinMode(buttonPin, INPUT);      

  //init the array with 255, a value that can never happen-- so we can check boundaries
  //when we loop through an array this big and don't have this many devices.
  for (x=0;x<MaxAddress;x++)
  {
    SlaveDevices[x] = 255;
  };
  
  ClearLCD(); //clear the LCD
  
  ScanI2C(); //scan the I2C bus for attached devices.
  CurrentAddress = 1; //set to the FIRST item
  SlaveAddress = SlaveDevices[CurrentAddress];  //set to the FIRST item
}

void loop()
{
  ClearLCD();
  
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {     
    // turn LED on:    
    CurrentAddress++;
    
    SlaveAddress = SlaveDevices[CurrentAddress];
    
    Serial.print("Current Address: ");
    Serial.println(SlaveDevices[CurrentAddress]);

    //increment through the available I2C devices
    if (SlaveAddress > MaxAddress){
      CurrentAddress = 1; //set to the FIRST item
      SlaveAddress = SlaveDevices[CurrentAddress];
    };
    
    //check for the init value, if it is the init value, start looping again from the beginning
    if (SlaveAddress == 255){
      CurrentAddress = 1; //set to the FIRST item
      SlaveAddress = SlaveDevices[CurrentAddress];
    };

   Serial.print("Current Address: ");
   Serial.println(SlaveDevices[CurrentAddress]);

  } 


  Wire.requestFrom(SlaveAddress, 6);    // request 6 bytes from slave device #2
  

   
  lcd.setCursor(0,0);
  lcd.print("Requestnum:");
  lcd.print(requestnum);
  lcd.setCursor(1,4);
  lcd.print("                  ");
  lcd.setCursor(1,4);
  lcd.print("Slave Addr:");
  lcd.print(SlaveAddress);
  requestnum++;

  while(Wire.available())    // slave may send less than requested
  {
 
    int c = Wire.read(); // receive a byte as character
    Serial.println("Recieved ");         // print the character
    Serial.println(c);         // print the character


  }

  delay(500);
}

void ClearLCD()
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(1,4);
  lcd.print("                ");

}

void ScanI2C()
{
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 0; address < MaxAddress; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      lcd.setCursor(0,0);
      lcd.print("                  ");
      lcd.setCursor(0,0);
      lcd.print("Scanning...");
      lcd.setCursor(1,4);
      lcd.print("                  ");
      lcd.setCursor(1,4);
      lcd.print("I2C addr 0x");
      lcd.print(address, HEX);

      //Delay so we see what is going on
      delay (1500);

      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
      SlaveDevices[nDevices] = address;
    }
    else if (error==4)
    {
      Serial.print("Unknow error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  //temp only....
      nDevices++;
      SlaveDevices[nDevices] = 15;
      nDevices++;
      SlaveDevices[nDevices] = 17;
      nDevices++;
      SlaveDevices[nDevices] = 40;
      nDevices++;
      SlaveDevices[nDevices] = 55;
  // end temp
  
  //set the max devices to the highest device found
  MaxAddress = SlaveDevices[nDevices];
 
 
  
}

