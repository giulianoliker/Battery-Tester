/* 
  BATTERY TESTER
  By Giuliano Liker

  This program measures the voltage of the battery and logs the time and the voltage into the file on the SD card in predetermied intervals.
  The idea is to provide a constant load on the battery and measure performance for comparisson purpose.

  TO DO: During startup, display data saving time interval. ("Save data every 5 min 30 sec")
  TO DO: Automatically stop measurement when the voltage is zero for more than 1 minute.
  TO DO: Display error messages on the LCD
*/

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>
#include <Time.h>

const int SDpin = 10;                           // Pin 10 on Arduino Uno
const byte batteryOnePin = A0;                  // Pin for battery 1
const byte batteryTwoPin = A1;                  // Pin for battery 2
float voltageBatteryOne;                        // Store measured voltage for battery 1
float voltageBatteryTwo;                        // Store measured voltage for battery 2
int voltageCheckInterval = 1000;                // How often to check the voltage (miliseconds)
int dataSaveInterval = 5000;                    // How often to log data to the SD card
int clockRefreshInterval = 1000;                // How often to refresh the clock
unsigned long previousVoltageCheckInterval = 0; // declare variable for interval function
unsigned long previousDataSaveInterval = 0;     // declare variable for interval function
unsigned long previousClockRefreshInterval = 0; // declare variable for interval function
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);            // initialize the library with the numbers of the interface pins

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

  // LCD data logging interval message
  int saveSec = dataSaveInterval / 1000;
  int saveMin = saveSec / 60;
  saveSec %= 60;
  saveMin %= 60;  
  lcd.setCursor(0,0);
  lcd.print("Save data every");
  lcd.setCursor(0,1);
  lcd.print(saveMin);
  lcd.setCursor(3,1);
  lcd.print("min");
  lcd.setCursor(7,1);
  lcd.print(saveSec);
  lcd.setCursor(10,1);
  lcd.print("sec");
  delay(5000);
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

void printVoltageToLCD()
{
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

void saveToSD()
{
  // Open the file
  File dataFile = SD.open("batTest.txt", FILE_WRITE);

  // If the file is available, write to it
  if (dataFile)
  {
    dataFile.print(hour());
    dataFile.print(":");
    dataFile.print(minute());
    dataFile.print(":");
    dataFile.print(second());
    dataFile.print(",");
    dataFile.print(voltageBatteryOne);
    dataFile.print(",");
    dataFile.println(voltageBatteryTwo);
    dataFile.close();
  }
  // If unable to read the file, display an error
  else
  {
    Serial.println("Error opening file on the SD card!");
  }
}

void loop()
{
  unsigned long currentTime = millis();

  // Calculate voltage and display on serial and LCD
  if (currentTime - previousVoltageCheckInterval >= voltageCheckInterval)
  {
    previousVoltageCheckInterval = currentTime;

    // Calculate voltage
    voltageBatteryOne = (analogRead(A0) * 5.00) / 1023.00;
    voltageBatteryTwo = (analogRead(A1) * 5.00) / 1023.00;

    // Display volate and time on serial
    printToSerial();

    // Display battery voltage on the LCD
    printVoltageToLCD();
  }

  // Save data to the SD card
  if (currentTime - previousDataSaveInterval >= dataSaveInterval)
  {
    previousDataSaveInterval = currentTime;

    // Save data to SD card
    saveToSD();
  }

  // Display time on the LCD
  if (currentTime - previousClockRefreshInterval >= clockRefreshInterval)
  {
    previousClockRefreshInterval = currentTime;

    // Display elapsed time on the LCD
    printTimeToLCD();
  }
}