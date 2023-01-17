#include <USI_TWI_Master.h>
#include <TinyWireM.h>


//#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define LED 1

//negative used due to direction of accelerometer
#define upperBound -1.3
#define lowerBound -0.7

Adafruit_MMA8451 mma = Adafruit_MMA8451();

float X;
float Y;
float Z;

int threshhold;
float avgX = 0;
int samples = 10;

void setup() {

  if (! mma.begin()) {
    while (1);
  }

  mma.setRange(MMA8451_RANGE_2_G);

  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
}

void loop() {
  /* Get a new sensor event */
  sensors_event_t event;
  mma.getEvent(&event);

  X = event.acceleration.x;
  Y = event.acceleration.y;
  Z = event.acceleration.z;

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
