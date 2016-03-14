//We always have to include the library
#include "LedControl.h"
// Distance sensor pins
#define trigPin 9
#define echoPin 8
/*
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
*/
LedControl lc=LedControl(12,11,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=100;
unsigned int cnt = 0;
/* sensor calibration from fluid level, in CM
   This is how far your sensor is from the actual, maximum fluid level */
unsigned long calibration = 10;

/* bytecode for the 8x8 images */
static byte waterlevel[][8] = {
{B00000000,B01100101,B10010101,B10010110,B10010110,B10010101,B01100101,B00000000}, // OK
{B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111111}, // water level, low
{B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B10101101,B11111111},
{B00000000,B00000000,B00000000,B00000000,B00000000,B10101101,B11111111,B11111111},
{B00000000,B00000000,B00000000,B00000000,B11010011,B11111111,B11111111,B11111111},
{B00000000,B00000000,B00000000,B10111010,B11111111,B11111111,B11111111,B11111111},
{B00000000,B00000000,B10110101,B11111111,B11111111,B11111111,B11111111,B11111111},
{B00000000,B10110110,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111},
{B11010101,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111},
{B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111} // water level, high
};

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,2);
  /* and clear the display */
  lc.clearDisplay(0);

  /* Distance sensor */
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void showlevel(unsigned int cnt) {
  unsigned int i=0;
    for (i=0;i<=7;i++) { /* 8 rows, display 0..7 */
    lc.setRow(0,i,waterlevel[cnt][i]);
  }  
}

void loop() { 
  long duration, distance;
  digitalWrite(trigPin, LOW);   // get ready to measure
  delayMicroseconds(2);         // wait
  digitalWrite(trigPin, HIGH);  // send measuring signal
  delayMicroseconds(10);        // for 10us
  digitalWrite(trigPin, LOW);   // end signal
  duration = pulseIn(echoPin, HIGH);  // wait for input signal (rebound)
  distance = (duration/2) / 29.1;     // calculate speed of sound to meters, speed is ~340ms, ie. ~1.7cm per us

  cnt = 0;
  if (distance < 50 + calibration) {cnt++;}
  if (distance < 40 + calibration) {cnt++;}
  if (distance < 35 + calibration) {cnt++;}
  if (distance < 30 + calibration) {cnt++;}
  if (distance < 25 + calibration) {cnt++;}
  if (distance < 20 + calibration) {cnt++;}
  if (distance < 15 + calibration) {cnt++;}
  if (distance < 10 + calibration) {cnt++;}
  if (distance < 5 + calibration) {cnt++;}

  lc.setIntensity(0,2+cnt/2); // increase intensity as distance shortens

  showlevel(cnt); // draw the image to the display
  delay(delaytime); // wait
  lc.clearDisplay(0); // clear display, or we will have LEDs that were on, stay that way
}
