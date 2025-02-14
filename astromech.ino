#include <Servo.h>
#include <RC_Receiver.h>

#define LEFT_PIN 12
#define RIGHT_PIN 13

// right joystick horiz
#define CH1 3
// right joystick vert
#define CH2 5
// left joystick vert
#define CH3 6
// left joystick horiz
#define CH4 9
// left knob
#define CH5 10
// right knob
#define CH6 11
#define HEAD_INT_3 7
#define HEAD_INT_4 8

Servo left;
Servo right;
RC_Receiver receiver(CH1, CH2, CH3, CH4, CH5, CH6);
int minMax[6][2] = 
{
	{1110,1995}, 
	{1117,1995}, 
	{1117,1990}, 
	{997,1905}, 
	{997,1995}, 
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
  pinMode(HEAD_INT_3, OUTPUT);
  pinMode(HEAD_INT_4, OUTPUT);
  receiver.setMinMax(minMax);
  left.attach(LEFT_PIN, 1000, 2000);
  right.attach(RIGHT_PIN, 1000, 2000);
  stopHead();
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
  if (steeringVal < 1600) { // turning left
    s_vals.leftMotorVal *= float(map(steeringVal, 1114, 1614, 30, 100) / 100.0);
  } else if (steeringVal > 1625) { // turning right
    rightTurnPercent = float(map(steeringVal, 1614, 1995, 100, 30)) / 100.0;
    s_vals.rightMotorVal *= rightTurnPercent;
  }
}

void stopHead() {
  digitalWrite(HEAD_INT_3, LOW);
  digitalWrite(HEAD_INT_4, LOW);
}

// potval is ch5
void moveHead(int potVal) {
  // Turn head left
  if (potVal > 995 && potVal <= 1329) {
    digitalWrite(HEAD_INT_3, LOW);
    digitalWrite(HEAD_INT_4, HIGH);
  } else if (potVal > 1329 && potVal <= 1663) {
    stopHead();
  } else if (potVal > 1663 && potVal <= 1995) {
    digitalWrite(HEAD_INT_4, LOW);
    digitalWrite(HEAD_INT_3, HIGH);
  } else {
    stopHead();
  }
}

void displayRadioChannels() {
  Serial.print(receiver.getRaw(1));
  Serial.print('\t');
  Serial.print(receiver.getRaw(2));
  Serial.print('\t');
  Serial.print(receiver.getRaw(3));
  Serial.print('\t');
  Serial.print(receiver.getRaw(4));
  Serial.print('\t');
  Serial.print(receiver.getRaw(5));
  Serial.print('\t');
  Serial.print(receiver.getRaw(6));
  Serial.println('\t');
}

int motorVal;
void loop() {
  motorVal = nearest2(map(receiver.getRaw(3), 1116, 2000, 15, 60));
  s_vals.leftMotorVal = motorVal + 6;
  s_vals.rightMotorVal = motorVal;
  calcSteering(nearest2(receiver.getRaw(1)));
  left.write(s_vals.leftMotorVal);
  right.write(s_vals.rightMotorVal);
  // displayRadioChannels();
  moveHead(receiver.getRaw(5));
}