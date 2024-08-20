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
	{1110,1995}, 
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

typedef struct {
  int leftMotorVal;
  int rightMotorVal;
} Steering;

Steering s_vals;
float rightTurnPercent;

void calcSteering(int steeringVal) {
  if (steeringVal < 1610) { // turning left
    s_vals.leftMotorVal *= float(map(steeringVal, 1114, 1614, 30, 100) / 100.0);
  } else if (steeringVal > 1620) { // turning right
    rightTurnPercent = float(map(steeringVal, 1614, 1995, 100, 30)) / 100.0;
    s_vals.rightMotorVal *= rightTurnPercent;
  }
}

int motorVal;
void loop() {
  motorVal = nearest2(map(receiver.getRaw(3), 1116, 2000, 15, 180));
  s_vals.leftMotorVal = motorVal;
  s_vals.rightMotorVal = motorVal; // for some reason the right motor starts 6 units ahead of the left
  calcSteering(nearest2(receiver.getRaw(1)));
  left.write(s_vals.leftMotorVal);
  right.write(s_vals.rightMotorVal - 6);
}