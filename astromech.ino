#include <Servo.h>

#define LEFT_PIN 2
#define RIGHT_PIN 3
#define CH1 4
#define CH2 5
#define CH3 6
#define CH4 7
#define CH5 8
#define CH6 9

Servo left;
Servo right;

void calibrateESC() {
  stopMotors();
  delay(800);
  left.write(180);
  right.write(180);
  delay(100);
  stopMotors();
}

void stopMotors() {
  left.write(0);
  right.write(0);
}

void setup() {
  Serial.begin(9600);
  left.attach(2, 1000, 2000);
  right.attach(3, 1000, 2000);
  calibrateESC();
  Serial.println("arming complete");

}



void loop() {
}