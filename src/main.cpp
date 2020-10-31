/* 
  BATTERY TESTER
  By Giuliano Liker

  This program measures the voltage of the battery and logs the time and the voltage into the file on the SD card in predetermied intervals.
  The idea is to provide a constant load on the battery and measure performance for comparisson purpose.

  TO DO: Automatically stop measurement when the voltage is zero for more than 1 minute.
  TO DO: Add blinking character on the LCD to notify that the process is in progress.
  TO DO: Add timer on screen to notify when the next measurement is going to happen.
  TO DO: Add ability to track multiple batteries at the same time.
  TO DO: Display error messages on the LCD
*/

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>
#include <Time.h>

const int SDpin = 10;                // Pin 10 on Arduino Uno
const byte batteryOnePin = A0;       // Pin for battery 1
const byte batteryTwoPin = A1;       // Pin for battery 2
float voltageBatteryOne;             // Store measured voltage for battery 1
float voltageBatteryTwo;             // Store measured voltage for battery 2
int timeCheckInterval = 1000;        // How often to check the voltage (miliseconds)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // initialize the library with the numbers of the interface pins

void setup()
{

  Serial.begin(9600);
  pinMode(batteryOnePin, INPUT); // Input for battery 1
  pinMode(batteryOnePin, INPUT); // Input for battery 2
  pinMode(SDpin, OUTPUT);        // Set output to the SD card pin

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

// Add leading zero to LCD if the time value is in the single digit
void printDigitsLCD(int digits)
{
  if (digits < 10)
  {
    lcd.print('0');
  }
  lcd.print(digits);
}

void printTimeToLCD()
{
  lcd.setCursor(12, 0);
  lcd.print("TIME");
  lcd.setCursor(8, 1);
  printDigitsLCD(hour());
  lcd.setCursor(10, 1);
  lcd.print(":");
  lcd.setCursor(11, 1);
  printDigitsLCD(minute());
  lcd.setCursor(13, 1);
  lcd.print(":");
  lcd.setCursor(14, 1);
  printDigitsLCD(second());
}

// Add leading zero to Serial if the time value is in the single digit
void printDigitsSerial(int digits)
{
  if (digits < 10)
  {
    Serial.print('0');
  }
  Serial.print(digits);
}

void printToSerial()
{
  Serial.print("Voltage: ");
  Serial.print(voltageBatteryOne);
  Serial.print("V");
  Serial.print("\t");
  Serial.print("Voltage: ");
  Serial.print(voltageBatteryTwo);
  Serial.print("V");
  Serial.print("\t");
  Serial.print("Time: ");
  printDigitsSerial(hour());
  Serial.print(":");
  printDigitsSerial(minute());
  Serial.print(":");
  printDigitsSerial(second());
  Serial.println();
}

void loop()
{
  // Calculate voltage
  voltageBatteryOne = (analogRead(A0) * 5.00) / 1023.00;
  voltageBatteryTwo = (analogRead(A1) * 5.00) / 1023.00;

  // Display values on serial
  printToSerial();

  // Display battery 1 values on the LCD
  lcd.clear();
  lcd.print("1:");
  lcd.setCursor(2, 0);
  lcd.print(voltageBatteryOne);
  lcd.setCursor(6, 0);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("2:");
  lcd.setCursor(2, 1);
  lcd.print(voltageBatteryTwo);
  lcd.setCursor(6, 1);
  lcd.print("V");


  // Display elapsed time on the LCD
  printTimeToLCD();

  // Open the file
  File dataFile = SD.open("batTest.txt", FILE_WRITE);

  // If the file is available, write to it
  if (dataFile)
  {
    dataFile.print(hour());
    dataFile.print(":");
    dataFile.print(minute());
    dataFile.print(",");
    dataFile.println(voltageBatteryOne);
    dataFile.close();
  }
  // If unable to read the file, display an error
  else
  {
    Serial.println("Error opening file on the SD card!");
  }

  delay(timeCheckInterval);
}
