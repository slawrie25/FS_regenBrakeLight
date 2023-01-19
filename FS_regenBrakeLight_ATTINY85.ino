/*
* @Author Struan Lawrie
*
* Requires ATTinyCore board files NOT standard ATTiny board files
* https://github.com/SpenceKonde/ATTinyCore/releases/tag/1.5.2
* 
* Currently using 0x1D for MMA8451 address, A pin is tied to ground
* 
* Wiring is as follows:
* ATTiny pin:
* 
* #1(RESET/A0)
* #2(A3)
* #3(A2)
* #4(GND)-----------------GND--------------PWR_GND
* #5(SDA/0)---------------MMA_SDA
* #6(1)-------------------LED
* #7(SCL/A1)--------------MMA_SCL
* #8(VCC)-----------------MMA_VCC----------PWR_5V
*                         MMA_A------------GND
*/

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define LED 1

//negative used due to direction of accelerometer (<-X)
#define upperBound -1.3
#define lowerBound -0.7

Adafruit_MMA8451 mma = Adafruit_MMA8451();

float X;
//float Y;
//float Z;

int threshhold;
float avgX = 0;
int samples = 10;

void setup() {

  //flash LED once to show power on
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);

  //attempt to connect to MMA8451
  if (! mma.begin()) {
    while (1);
  }

  //set range of accelerometer to 4G
  mma.setRange(MMA8451_RANGE_4_G);

  //flash LED twice to show successful accelerometer connection
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
}

void loop() {
  /* Get a new sensor event */
  sensors_event_t event;
  mma.getEvent(&event);

  //variable to store acceleration value
  X = event.acceleration.x;
  //Y = event.acceleration.y;
  //Z = event.acceleration.z;

  //take an average for more accurate data
  avgX = 0;
  for (int i = 0; i < samples; i++) {
    sensors_event_t event;
    mma.getEvent(&event);
    avgX += event.acceleration.x;
  }
  X = avgX / samples;

  //use bounding for less light flicker
  if (X < upperBound) {
    digitalWrite(LED, HIGH);
  }
  else if(X > lowerBound ){
    digitalWrite(LED, LOW);
  }

  delay(10);
}
