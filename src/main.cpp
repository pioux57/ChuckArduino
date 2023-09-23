/*
    This code has been written for Hydra Airsoft Experience project by Christian Poupounot.
    It handle RBG leds with different color modes set using potentiometer.
*/

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Hardware
#define POTENTIOMETER_MAX   668         // Valeur maximum du potentiometre 668
#define POTENTIOMETER_MIN   30          // Valeur minimum du potentiometre
#define TEST_LED_DURATION   1000        // Durée d'allumage des leds en phase de test
const bool COMMON_CATHODE   = false;    // Depends on LEDs type you have for your make
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
LiquidCrystal_I2C lcd(0x27, 20, 4);


// Working variables
float       potPercentage   = 0;
uint8_t     filledSquare[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; // Custom char
byte        i;
bool        blinkOn         = true;     // Blink management
long int    lastBlink       = 0;        // Blink management
float       potarValue      = 0;

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
    lcd.clear();
    lcd.setCursor(0,0);

    // Serial.begin(9600);
    // Serial.println(F(""));
    lcd.print("Starting...         ");
    lcd.setCursor(0,2);
    lcd.print("    Hydra Airsoft   ");

    pinMode(PIN_LED_R1, OUTPUT);
    pinMode(PIN_LED_G1, OUTPUT);
    pinMode(PIN_LED_B1, OUTPUT);
    pinMode(PIN_LED_R2, OUTPUT);
    pinMode(PIN_LED_G2, OUTPUT);
    pinMode(PIN_LED_B2, OUTPUT);
    displayColor(0,0,0);
    delay(2000);

    // Test sequence on start to check if there is some burned leds
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(F("- Test Red          "));
    displayColor(255, 0, 0);
    delay(TEST_LED_DURATION);
    lcd.setCursor(0,1);
    lcd.print(F("- Test Green        "));
    displayColor(0, 255, 0);
    delay(TEST_LED_DURATION);
    lcd.setCursor(0,1);
    lcd.print(F("- Test Blue         "));
    displayColor(0, 0, 255);
    delay(TEST_LED_DURATION);
    lcd.setCursor(0,1);
    lcd.print(F("- Test Orange       "));
    displayColor(255, 165, 0);
    delay(TEST_LED_DURATION);
    lcd.setCursor(0,1);
    lcd.print(F("- Test Purple       "));
    displayColor(255, 0, 255);
    delay(TEST_LED_DURATION);
    displayColor(0,0,0);

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
    potarValue = POTENTIOMETER_MAX - (float)analogRead(PIN_POTAR);
    potPercentage = potarValue/POTENTIOMETER_MAX;
    if (potPercentage < 0) {potPercentage = 0;}
    potPercentage = potPercentage * 100;

    // Display on LCD
    lcd.setCursor(0,0);
    lcd.print(F("       "));
    lcd.print(potPercentage);
    lcd.print(F("%        "));

    // Progress bar display
    lcd.setCursor(0,2);
    for (i = 0 ; i < 20 ; i++){
        if (i < potPercentage/5) {lcd.print((char)0);}
        else {lcd.print(" ");}
    }

    // DEBUG for potentiometer calibration
    //lcd.setCursor(0,3);
    //lcd.print(analogRead(PIN_POTAR));
    //lcd.print("    ");

    // Lighting RGB Leds
    if (potPercentage > 3){
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
    } else {
        displayColor(0,0,0);
    }

    // UI Debouncing
    //delay(300);  
}

// Actual display of selected combination of colors
void displayColor(byte redValue, byte greenValue, byte blueValue) {

    if (COMMON_CATHODE){
        // Common cathode
        analogWrite(PIN_LED_R1, redValue);
        analogWrite(PIN_LED_G1, greenValue);
        analogWrite(PIN_LED_B1, blueValue);
        analogWrite(PIN_LED_R2, redValue);
        analogWrite(PIN_LED_G2, greenValue);
        analogWrite(PIN_LED_B2, blueValue);
    } else {
        // Common anode
        analogWrite(PIN_LED_R1, ~redValue);
        analogWrite(PIN_LED_G1, ~greenValue);
        analogWrite(PIN_LED_B1, ~blueValue);
        analogWrite(PIN_LED_R2, ~redValue);
        analogWrite(PIN_LED_G2, ~greenValue);
        analogWrite(PIN_LED_B2, ~blueValue);
    }
}
