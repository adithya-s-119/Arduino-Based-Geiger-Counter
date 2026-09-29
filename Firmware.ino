// Include Libraries
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_GFX.h>

// Pin Definitions
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
#define LOGO16_GLCD_HEIGHT  16
#define LOGO16_GLCD_WIDTH   16

// Global variables and defines
unsigned long currentCount; //variable for GM Tube events
unsigned long previousMillis; //variable for measuring time
unsigned int multiplier;  //variable for calculation CPM in this sketch
unsigned long cpm;        //variable for CPM
unsigned long averageCount;
float microsieverthour = 0;
float total_uSv = 0;
unsigned long cumulativeCount;
long count[61];
int i = 0;
int buttonA1 = 0;
float divider = 151;

// object initialization
#define LOG_PERIOD 15000  //Logging period in milliseconds
#define MAX_PERIOD 60000  //Maximum logging period 
#define SH1106_LCDHEIGHT 64
Adafruit_SH1106 oLed128x64(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define LOGO16_GLCD_HEIGHT  16
#define LOGO16_GLCD_WIDTH   16

void setup() {
  currentCount = 0;
  cpm = 0;
  averageCount = 0;
  multiplier = MAX_PERIOD / LOG_PERIOD;

  oLed128x64.begin(SH1106_SWITCHCAPVCC); 
  oLed128x64.clearDisplay();
  oLed128x64.display();

  pinMode(A1, INPUT);
  attachInterrupt(0, tube_impulse, FALLING); //define external interrupts
}
void loop()
{
  buttonA1 = digitalRead(A1);
  if (buttonA1 == HIGH) {
    buttonA1 = digitalRead(A1);
    currentCount = 0;
  }
  else if (buttonA1 == LOW) {

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > LOG_PERIOD) {
      previousMillis = currentMillis;
      cpm = currentCount * multiplier;
      microsieverthour = (cpm / divider);
      total_uSv = cumulativeCount/(60 * float(divider));

      count[i] = currentCount;
      i++;
      if (i == 61)
      {
        i = 0;
      }
      averageCount = currentCount - count[i]; // count[i] stores the value from 60 seconds ago
      averageCount = ((averageCount) / (1 - 0.00000333 * float(averageCount))); // accounts for dead time of the geiger tube.


      oLed128x64.setTextSize(1);
      oLed128x64.setTextColor(WHITE);
      oLed128x64.setCursor(35, 0);
      oLed128x64.println(F("Beta/Gamma"));
      oLed128x64.setCursor(0, 15);
      oLed128x64.println(F("CPM: "));
      oLed128x64.setCursor(23, 15);
      oLed128x64.println(cpm);
      oLed128x64.setCursor(0, 25);
      oLed128x64.println(microsieverthour);
      oLed128x64.setCursor(25, 25);
      oLed128x64.println(F(" uSv/hr"));
      oLed128x64.setCursor(0, 35);
      oLed128x64.println(F("Avg: "));
      oLed128x64.setCursor(25, 35);
      oLed128x64.println(averageCount);
      oLed128x64.setCursor(0, 45);
      oLed128x64.println(F("counts: "));
      oLed128x64.setCursor(42, 45);
      oLed128x64.println(cumulativeCount);
      oLed128x64.setCursor(0, 55);
      oLed128x64.println(total_uSv);
      oLed128x64.setCursor(25, 55);
      oLed128x64.println(F(" uSv"));
      oLed128x64.display();

      if (microsieverthour < 900 ) {

        oLed128x64.setCursor(75, 45);
        oLed128x64.println(F("Normal"));
        oLed128x64.setCursor(60, 55);
        oLed128x64.println(F("Background"));
        oLed128x64.display();
      }
      else if ((microsieverthour < 900) && (microsieverthour > 2000)) {

        oLed128x64.setCursor(75, 45);
        oLed128x64.println(F("Elevated"));
        oLed128x64.setCursor(60, 55);
        oLed128x64.println(F("Activity"));
        oLed128x64.display();
      }
      else if (microsieverthour < 2000 ) {

        oLed128x64.setCursor(75, 45);
        oLed128x64.println(F("Very"));
        oLed128x64.setCursor(60, 55);
        oLed128x64.println(F("High"));
        oLed128x64.display();
      }
      oLed128x64.clearDisplay();
      currentCount = 0;
      averageCount = 0;
    }
  }
}

void tube_impulse() {      //subprocedure for capturing events from Geiger Kit
  currentCount++;
  cumulativeCount++;
}
