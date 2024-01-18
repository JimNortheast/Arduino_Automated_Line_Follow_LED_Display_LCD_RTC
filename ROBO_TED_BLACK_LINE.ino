/*
***Combined code with Line Follow***
***RTC Clock Set***
***LCD Display***
***LED Lights***
***Battery Display***
*/


//Line Follow Include
#include <DFMobile.h>
DFMobile Robot(4, 5, 7, 6); //Setting motor pins on Romeo V2 Board
int RightValue;
int MiddleValue;
int LeftValue;
boolean shouldFollowLine = true;  // Flag to control line following





//RTC Include
#include <Wire.h>
#include <RTClib.h>
RTC_DS3231 rtc;
DateTime now;

//LCD Include
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//LED
#include <Adafruit_NeoPixel.h>
boolean shouldStopLEDs = false;  // Flag to control LED flashing

unsigned long previousMillis = 0;
const long interval = 500;  // Interval at which the rainbow pattern changes (milliseconds)
int currentColor = 0;


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN 12

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12


//LED
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const int NUM_COLORS = 4;
uint32_t colorArray[NUM_COLORS] = {
  pixels.Color(215, 215, 0),  // Yellow
  pixels.Color(255, 0, 100),  // Pink
  pixels.Color(100, 0, 255),  // Purple Blue
  pixels.Color(0, 0, 200)     // Blue
};
int currentColorIndex = 0;
uint32_t currentLEDColor = colorArray[currentColorIndex];
unsigned long previousLEDMillis = 0;
const long intervalLED = 75;  // Blink interval in milliseconds

//Battery Level include
/*#include "TM1651.h"
#define CLK 13  //pins definitions for TM1651 and can be changed to other ports
#define DIO A0
TM1651 batteryDisplay(CLK, DIO);
const float batteryVoltage = 7.4;       // Nominal voltage of the battery
const float voltageDividerRatio = 2.0;  // Voltage divider ratio
*/

void setup() {

  //RTC void setup
  Serial.begin(115200);
  Wire.begin();

  // Initialize the RTC
if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }


  //LCD void setup
  lcd.init();  // initialize the lcd

  //LED
  pixels.begin();  // This initializes the NeoPixel library.
  pixels.getPixels();
  pixels.show();


  //pixels.setPixelColor(0, pixels.Color(215,215,0)); // 1st LED Yellow.
  //pixels.show();



  //pixels.setPixelColor(1, pixels.Color(255,0,100)); // 2nd LED Pink.
  //pixels.show();


  //pixels.setPixelColor(2, pixels.Color(100,0,255)); // 3rd LED Purple Blue.
  //pixels.show();


  //pixels.setPixelColor(3, pixels.Color(0,0,200)); // 4th LED Blue.
  //pixels.show();


  //pixels.setPixelColor(4, pixels.Color(0,0,200)); // 5th LED Blue
  //pixels.show();


  //pixels.setPixelColor(5, pixels.Color(100,0,255)); // 6th LED Purple Blue.
  //pixels.show();


  //pixels.setPixelColor(6, pixels.Color(255,0,100)); // 7th LED Pink.
  //pixels.show();


  //pixels.setPixelColor(7, pixels.Color(215,215,0)); // 8th LED Yellow.
  //pixels.show();



  //Battery Setup
  /*batteryDisplay.init();
  batteryDisplay.set(5);  //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
*/

  //Line Follow void setup
  Wire.begin();
  if (!rtc.begin()) {
    while (1)
      ;
  }
  Robot.Direction(LOW, HIGH);
  Serial.begin(115200);
}



void loop() {



  // RTC void loop
  now = rtc.now();  // Get current date and time

  int lastTwoDigitsOfYear = now.year() % 100;  // Calculate last two digits of the year
  Serial.print(lastTwoDigitsOfYear, DEC);      // Display last two digits of the year
  Serial.print('/');
  Serial.print(now.month(), DEC);  //month
  Serial.print('/');
  Serial.print(now.day(), DEC);  //date
  Serial.print(" (");
  Serial.print(now.dayOfTheWeek());  //day of week
  Serial.print(") ");
  Serial.print(now.hour(), DEC);  //hour
  Serial.print(':');
  Serial.print(now.minute(), DEC);  //minute
  Serial.print(':');
  Serial.print(now.second(), DEC);  //second
  Serial.print(' ');
  Serial.print(now.isPM() ? "PM" : "AM");
  Serial.println();

  // Print date and time on LCD
  Serial.print("Current time: ");
  lcd.print(now.hour(), DEC);  //hour
  lcd.print(':');
  lcd.print(now.minute(), DEC);  //minute
  lcd.print(now.isPM() ? " PM" : " AM");
  lcd.print(" ");
  lcd.print(now.day(), DEC);  //date
  lcd.print("/");
  lcd.print(now.month(), DEC);  //month
  lcd.print("/");
  lcd.print(now.year(), DEC);  //year



  //Battery
  //Conversion formula for voltage
  // Read the analog voltage
  /*int sensorValue = analogRead(A0);
  float analogVoltage = sensorValue * (5.0 / 1023.0);

  // Calculate battery voltage
  float batteryVoltageActual = analogVoltage * voltageDividerRatio;

  // Calculate battery level based on the actual voltage
  int batteryLevel = map(constrain(batteryVoltageActual, 6.0, 8.4), 6.0, 8.4, 0, 5);

  // Display battery level on TM1651
  batteryDisplay.frame(batteryLevel == 0 ? FRAME_OFF : FRAME_ON);  // Turn off frame when battery is at its lowest level
  batteryDisplay.displayLevel(batteryLevel);
*/


  //Line Follow Loop
  DateTime now = rtc.now();
  int currentHour = now.hour();
  int currentMinute = now.minute();

  RightValue = digitalRead(8);
  MiddleValue = digitalRead(9);
  LeftValue = digitalRead(10);

  if ((currentHour == 9 && currentMinute == 0) || (currentHour == 15 && currentMinute == 0)) {
    shouldFollowLine = true;
  } else {
    shouldFollowLine = false;
  }

  //reading 3 pins values of Line Tracking Sensor
  RightValue = digitalRead(8);
  MiddleValue = digitalRead(9);
  LeftValue = digitalRead(10);

  //print out the value you read:
  Serial.print(LeftValue);
  Serial.print(MiddleValue);
  Serial.println(RightValue);
  //millis();

  // Check if the robot stops at the diagonal line
  if ((LeftValue == LOW) && (RightValue == LOW)) {
    if ((currentHour == 9 && currentMinute == 0) || (currentHour == 15 && currentMinute == 0)) {
      Robot.Speed(120, 120);   // Continue forward at set time
      shouldStopLEDs = false;  // LEDs should keep flashing
    } else {
      Robot.Speed(0, 0);      // Stop on the diagonal line
      shouldStopLEDs = true;  // LEDs should stop flashing
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, 0);  // Turn off LEDs
      }
      pixels.show();
 
    }
  } else {
    // Check if the robot is located in the middle of the line
    if ((MiddleValue == LOW)) {
      Robot.Speed(120, 120);  // Go straight
    }
    
    // Check if the robot is located on the right side of the line
    else if ((RightValue == LOW)) {
      Robot.Speed(200, 10);  // Turn right
    }

    // Check if the robot is located on the left side of the line
    else if ((LeftValue == LOW)) {
      Robot.Speed(10, 200);  // Turn left
    }  

    // Flash LEDs if allowed
    if (!shouldStopLEDs) {
      unsigned long currentLEDMillis = millis();
      if (currentLEDMillis - previousLEDMillis >= intervalLED) {
        previousLEDMillis = currentLEDMillis;

        for (int i = 0; i < NUMPIXELS; i++) {
          int randomColorIndex = random(NUM_COLORS);  // Choose a random color index
          uint32_t randomLEDColor = colorArray[randomColorIndex];
          pixels.setPixelColor(i, randomLEDColor);
        }
        pixels.show();
      }
    } else {
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, 0);  // Turn off LEDs
      }
      pixels.show();
    }
    
  }


  // Print time on LCD
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("--- ROBO TED ---");
  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);  //hour
  lcd.print(':');
  lcd.print(now.minute(), DEC);  //minute
  lcd.print(now.isPM() ? "PM" : "AM");
  lcd.print(" ");
  lcd.print(now.day(), DEC);  //date
  lcd.print("/");
  lcd.print(now.month(), DEC);  //month
  lcd.print("/");
  lcd.print(lastTwoDigitsOfYear, DEC);  // Display last two digits of the year
  lcd.print("    ");
}
