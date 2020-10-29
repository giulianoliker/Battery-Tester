/* 
  BATTERY TESTER
  By Giuliano Liker

  This program measures the voltage of the battery and logs the time and the voltage into the file on the SD card in predetermied intervals.
  The idea is to provide a constant load on the battery and measure performance for comparisson purpose.
*/

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

int SDpin = 10;     // Pin 10 on Arduino Uno
float finalVoltage; // Store measured voltage

void setup()
{

  Serial.begin(9600);
  pinMode(A0, INPUT);     // Input for voltage measurement
  pinMode(SDpin, OUTPUT); // SD card pin

  // Open serial communications and wait for port to open
  while (!Serial)
  {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initializing SD card...");

  // Check if the card is present and can be initialized
  if (!SD.begin(SDpin))
  {
    Serial.println("Card failed, or not present!");

    while (1) // Check only once if the card is not present
      ;
  }
  Serial.println("Card initialized.");
}

void loop()
{
  // Calculate voltage
  finalVoltage = (analogRead(A0) * 5.00) / 1023.00;

  // Display values on serial
  Serial.print("Voltage = ");
  Serial.print(finalVoltage);
  Serial.println("V");

  // Write values to SD card
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("batTest.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile)
  {
    dataFile.println(finalVoltage);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("Error opening file on the SD card!");
  }

  delay(200);
}
