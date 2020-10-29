/* 
  BATTERY TESTER
  By Giuliano Liker

  This program measures the voltage of the battery and logs the time and the voltage into the file on the SD card in predetermied intervals.
  The idea is to provide a constant load on the battery and measure performance for comparisson purpose.

  TO DO: Automatically stop measurement when the voltage is zero for more than 1 minute.
*/

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

const int SDpin = 10;        // Pin 10 on Arduino Uno
const byte voltagePin = A0;  // Pin to measure voltage
float finalVoltage;          // Store measured voltage
int elapsedTime;             // Store elapsed time
int timeCheckInterval = 500; // How often to check the voltage (miliseconds)

void setup()
{

  Serial.begin(9600);
  pinMode(voltagePin, INPUT); // Input for voltage measurement
  pinMode(SDpin, OUTPUT);     // Set output to the SD card pin

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
  elapsedTime = millis() / 1000;

  // Display values on serial
  Serial.print("Voltage: ");
  Serial.print(finalVoltage);
  Serial.print("V");
  Serial.print("\t");
  Serial.print("Time: ");
  Serial.print(elapsedTime);
  Serial.println(" seconds");

  // Open the file
  File dataFile = SD.open("batTest.txt", FILE_WRITE);

  // If the file is available, write to it
  if (dataFile)
  {
    dataFile.print(finalVoltage);
    dataFile.print(",");
    dataFile.println(elapsedTime);
    dataFile.close();
  }
  // If unable to read the file, display an error
  else
  {
    Serial.println("Error opening file on the SD card!");
  }

  delay(timeCheckInterval);
}
