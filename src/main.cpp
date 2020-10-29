/* 
  BATTERY TESTER
  By Giuliano Liker

  This program measures the voltage of the battery and logs the time and the voltage into the file on the SD card in predetermied intervals.
  The idea is to provide a constant load on the battery and measure performance for comparisson purpose.

  TO DO: Automatically stop measurement when the voltage is zero for more than 1 minute.
  TO DO: Add blinking character on the LCD to notify that the process is in progress.
  TO DO: Show elapsed time on the LCD
  TO DO: Add timer on screen to notify when the next measurement is going to happen.
  TO DO: Add ability to track multiple batteries at the same time.
*/

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>
#include <Time.h>

const int SDpin = 10;                // Pin 10 on Arduino Uno
const byte voltagePin = A0;          // Pin to measure voltage
float finalVoltage;                  // Store measured voltage
int elapsedTime;                     // Store elapsed time
int timeCheckInterval = 1000;        // How often to check the voltage (miliseconds)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // initialize the library with the numbers of the interface pins

void setup()
{

  Serial.begin(9600);
  pinMode(voltagePin, INPUT); // Input for voltage measurement
  pinMode(SDpin, OUTPUT);     // Set output to the SD card pin

  // LCD start message
  lcd.begin(16, 2);
  lcd.print("BATTERY TESTER");
  lcd.setCursor(0, 1);
  lcd.print("By Giuliano");
  delay(2000);
  lcd.clear();

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

// Add preceding zero if the time value is in the single digit
void printDigits(int digits)
{
  if (digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

void printToLCD()
{
  lcd.setCursor(12, 0);
  lcd.print("TIME");
  lcd.setCursor(8, 1);
  printDigits(hour());
  lcd.setCursor(10, 1);
  lcd.print(":");
  lcd.setCursor(11, 1);
  printDigits(minute());
  lcd.setCursor(13, 1);
  lcd.print(":");
  lcd.setCursor(14, 1);
  printDigits(second());
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

  // Display battery 1 values on the LCD
  lcd.clear();
  lcd.print("BATT1");
  lcd.setCursor(0, 1);
  lcd.print(finalVoltage);
  lcd.setCursor(4, 1);
  lcd.print("V");

  // Display elapsed time on the LCD
  printToLCD();

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
