/*
  YAB (Yet Another Beerduino)

  Pins used:
    Fridge: 12
    LCD/Button 1 Wire Bus: 2
*/
 
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

/* For adding in the Ethernet or WiFi Shield */
//#include <SPI.h>
//#include <Ethernet.h>
//#include <WiFi.h>

/* LCD Colors */
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

/* One wire bus pin */
#define ONE_WIRE_BUS 2

/* Fridge States */
#define ON 1
#define OFF 0

/* Initialize LCD object */
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Arrays to hold temp devices 
// DeviceAddress insideThermometer, outsideThermometer;
DeviceAddress fridgeThermometer;

/* Define Fridge AC Relay Control Pin */
const int fridgePin = 12;

/* Temp sesnor polling interval (seconds) and display hold time */
const int pollingInterval = 5;

/* 
   Temperature range to cycle fridge in Celsius
   lowTemp = Temp reached to turn fridge off
   highTemp = Temp reached to turn fridge on
*/
const int lowTemp = 25;
const int highTemp = 30;

/* Variables to keep track of stuff */

/* Number of milliseconds since bootup */
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

/* Number of times the fridge has been cycled (off to on) */
int cycles = 0;
int fridgeState = 0;

/* Min and max temperatures seen */
int maxTemp = 0;
int minTemp = 0;

/* Variable to store buttons */
uint8_t buttons = 0;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  /* Turn off relay to Fridge */
  pinMode(fridgePin, OUTPUT);
  digitalWrite(fridgePin, LOW);
  
  // Initialize Temp Sensor Library
  sensors.begin();

  // Find some sensors
  if (!sensors.getAddress(fridgeThermometer, 0)) 
  {
    lcd.print("NO TEMP SENSORS");
    for(int count = 0; count < 5; ++count)
    {
      lcd.setBacklight(RED);
      delay(500);
      lcd.setBacklight(BLUE);
      delay(500); 
    }
  }
  lcd.setBacklight(WHITE);
  lcd.clear();
}

void loop() {
  buttons = lcd.readButtons(); 
  
  /* Get some temperature readings and do stuff */
  currentMillis = millis();
  if(currentMillis - previousMillis > pollingInterval * 1000)
  {
    previousMillis = currentMillis;
    sensors.requestTemperatures();
    lcd.clear();
    if(sensors.getAddress(fridgeThermometer, 0))
    {
      /* Check to see if we hit a new low or high temp */
      if(sensors.getTempC(fridgeThermometer) > maxTemp)
        maxTemp = sensors.getTempC(fridgeThermometer);
      if(sensors.getTempC(fridgeThermometer) < minTemp)
        minTemp = sensors.getTempC(fridgeThermometer);
      
      lcd.print(sensors.getTempC(fridgeThermometer));
      lcd.setCursor(0,0);
      lcd.print("Temp: ");
      lcd.print(sensors.getTempC(fridgeThermometer));
      lcd.print("C");
      /* If temperature is too high, turn on the fridge */
      if(sensors.getTempC(fridgeThermometer) > highTemp)
      {
        if(fridgeState == OFF)
          ++cycles;
        digitalWrite(fridgePin, HIGH);
        fridgeState = ON;
        lcd.setCursor(0,1);
        lcd.print("Fridge On");
        lcd.setBacklight(BLUE);
      }
      /* If the temperature is too low, turn off the fridge */
      else if(sensors.getTempC(fridgeThermometer) < lowTemp)
      {
        digitalWrite(fridgePin, LOW);
        fridgeState = OFF;
        lcd.setCursor(0,1);
        lcd.print("Fridge Off");
        lcd.setBacklight(GREEN);
      }    
    }
    else
    {
      lcd.setBacklight(RED);
      displayInfo("NO TEMP SENSORS", "");
    }
  }
  
  /* Process button input */
  if (buttons)
  {
    if (buttons & BUTTON_LEFT)
      displayInfo("CFG Temp Range:", String(lowTemp) + "-" + String(highTemp) + " C");
    if (buttons & BUTTON_RIGHT)
      displayInfo("Min/Max Temps:", String(minTemp) + "-" + String(maxTemp) + " C");
    if (buttons & BUTTON_UP)
      displayInfo("Fridge Cycles:", String(cycles));
    if (buttons & BUTTON_DOWN)
      displayInfo("Uptime (Secs):", String(millis() / 1000));
  }
}

void displayInfo(String topText, String bottomText)
{
  lcd.clear();
  previousMillis = millis();
  lcd.setCursor(0,0);
  lcd.print(topText);
  lcd.setCursor(0,1);
  lcd.print(bottomText);
}
