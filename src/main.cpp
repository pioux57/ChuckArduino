/*
    This code has been written for Hydra Airsoft Experience project by Christian Poupounot.
    It handle RBG leds with different color modes set using potentiometer.
*/

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Hardware
#define POTENTIOMETER_MAX   940         // Valeur maximum du potentiometre
#define POTENTIOMETER_MIN   30          // Valeur minimum du potentiometre
const bool COMMON_CATHODE   = true; // Depends on LEDs type you have for your make
// First serie of 3 RGB LEDs
const int PIN_LED_R1        = 9;
const int PIN_LED_G1        = 10;
const int PIN_LED_B1        = 11;
// Second serie of 3 RGB LEDs
const int PIN_LED_R2        = 3;
const int PIN_LED_G2        = 5;
const int PIN_LED_B2        = 6;
// Setting potentiometer
const int PIN_POTAR         = A1;
// LCD screen
LiquidCrystal_I2C lcd(0x3F, 20, 2);


// Working variables
float       potPercentage   = 0;
uint8_t     filledSquare[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; // Custom char
byte        i;
bool        blinkOn         = true;     // Blink management
long int    lastBlink       = 0;        // Blink management

// Color management
enum RGB{
  RED,
  GREEN,
  BLUE,
  NUM_COLORS
};
int _rgbLedValues[] = {255, 0, 0}; // Red, Green, Blue

// Functions declaration
void displayColor(byte r, byte g, byte b);

void setup() {
 	// LCD Screen init
 	lcd.init();
 	lcd.backlight();
    lcd.createChar(0, filledSquare); // Sends the custom char to lcd
    lcd.setCursor(0,0);

    // Serial.begin(9600);
    // Serial.println(F(""));
    lcd.println(F("Starting...         "));
    lcd.println(F("    Hydra Airsoft   "));

    pinMode(PIN_LED_R1, OUTPUT);
    pinMode(PIN_LED_G1, OUTPUT);
    pinMode(PIN_LED_B1, OUTPUT);
    pinMode(PIN_LED_R2, OUTPUT);
    pinMode(PIN_LED_G2, OUTPUT);
    pinMode(PIN_LED_B2, OUTPUT);

    // Test sequence on start to check if there is some burned leds
    lcd.setCursor(0,1);
    lcd.println(F("- Test Red          "));
    displayColor(255, 0, 0);
    delay(500);
    lcd.setCursor(0,1);
    lcd.println(F("- Test Green        "));
    displayColor(0, 255, 0);
    delay(500);
    lcd.setCursor(0,1);
    lcd.println(F("- Test Blue         "));
    displayColor(0, 0, 255);
    delay(500);
    lcd.setCursor(0,1);
    lcd.println(F("- Test Orange       "));
    displayColor(255, 165, 0);
    delay(500);
    lcd.setCursor(0,1);
    lcd.println(F("- Test Purple       "));
    displayColor(0, 255, 255);
    delay(500);

    // Quick blink to show that tests are passed
    signalBlink(100,2);

    // Set initial fading color
    displayColor(_rgbLedValues[RED], _rgbLedValues[GREEN], _rgbLedValues[BLUE]);
    delay(100);
    lcd.clear();
}

void loop() {
    // Blink management
    if (millis() - lastBlink >= 500){
        if (blinkOn) {blinkOn = false;}
        else {blinkOn = true;}
        lastBlink = millis();
    }

    // Computing percentage and reading potentiometer value
    potPercentage = (float)analogRead(PIN_POTAR)/POTENTIOMETER_MAX;
    potPercentage = potPercentage * 100;

    // Display on LCD
    lcd.setCursor(0,0);
    lcd.print(F("         "));
    lcd.print(potPercentage);
    lcd.print(F("%        "));
    lcd.setCursor(0,1);
    lcd.print(F("                    "));

    for (i = 0 ; i++ ; i < potPercentage/5){
        lcd.print((char)0); // Custom char
    }

    // Lighting RGB Leds
    if (potPercentage > 80) {
        if (blinkOn) {
            displayColor(255,0,0); // RED 
        } else {
            displayColor(0,0,0); // OFF
        }
        
    } else {
        if (potPercentage > 50) {
            displayColor(255,165,0); // ORANGE
        } else {
            displayColor(0,255,0); // GREEN
        }
    }

    // UI Debouncing
    delay(300);  
}

// Actual display of selected combination of colors
void displayColor(byte redValue, byte greenValue, byte blueValue) {

    if (COMMON_CATHODE){
        // Common cathode
        analogWrite(PIN_LED_R1, redValue);
        analogWrite(PIN_LED_G1, greenValue);
        analogWrite(PIN_LED_B1, blueValue);
        if (high_brightness){
            analogWrite(PIN_LED_R2, redValue);
            analogWrite(PIN_LED_G2, greenValue);
            analogWrite(PIN_LED_B2, blueValue);
        }
    } else {
        // Common anode
        analogWrite(PIN_LED_R1, ~redValue);
        analogWrite(PIN_LED_G1, ~greenValue);
        analogWrite(PIN_LED_B1, ~blueValue);
        if (high_brightness){
            analogWrite(PIN_LED_R2, ~redValue);
            analogWrite(PIN_LED_G2, ~greenValue);
            analogWrite(PIN_LED_B2, ~blueValue);
        }
    }

    if (!high_brightness){
        analogWrite(PIN_LED_R2, 0);
        analogWrite(PIN_LED_G2, 0);
        analogWrite(PIN_LED_B2, 0);
    }
}
