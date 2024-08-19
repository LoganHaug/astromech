#include <Servo.h>
#include <RC_Receiver.h>

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
RC_Receiver receiver(CH1, CH2, CH3, CH4, CH5, CH6);
int minMax[6][2] = 
{
	{1117,1995}, 
	{1117,1995}, 
	{1117,1990}, 
	{997,1905}, 
	{1997,1995}, 
	{997,1995}
};

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
  receiver.setMinMax(minMax);
  left.attach(2, 1000, 2000);
  right.attach(3, 1000, 2000);
  calibrateESC();
  Serial.println("arming complete");

}

int nearest2(int num) {
  return num - (num % 2);
}

int motorVal;
void loop() {
  motorVal = nearest2(map(receiver.getRaw(3), 1116, 2000, 0, 180));
  Serial.println(motorVal);
  left.write(motorVal);
  right.write(motorVal);
}