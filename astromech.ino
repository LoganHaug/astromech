#include <Adafruit_PWMServoDriver.h>
#include <RC_Receiver.h>
#include <Wire.h>

// PCA9685
Adafruit_PWMServoDriver motor_driver = Adafruit_PWMServoDriver();


// right joystick horiz
#define CH1 3
// right joystick vert
// #define CH2
// left joystick vert
#define CH3 5
// left joystick horiz
// #define CH4 
// left knob
#define CH5 6
// right knob
// #define CH6 
#define HEAD_INT_3 7
#define HEAD_INT_4 8

// Fly sky FS-IA6
RC_Receiver receiver(CH1, CH3, CH5);
int minMax[3][2] = 
{
	{1116,1996}, 
	{1096,1995},  
	{997,1995}, 
};

void setup() {
  Serial.begin(9600);
  delay(50);
  Serial.println("arming");
  pinMode(HEAD_INT_3, OUTPUT);
  pinMode(HEAD_INT_4, OUTPUT);
  stopHead();
  motor_driver.begin();
  motor_driver.setOscillatorFrequency(26500000);  // chip dependent, test for pwm hz
  motor_driver.setPWMFreq(50); // PWM Freq of rc electric parts classic esc 30A
  Wire.setClock(100000); // Normal clock hz
  delay(100); // allow i2c device to initialize
  motor_driver.setPWM(0, 0, 207);
  delay(200);
  motor_driver.setPWM(0, 0, 410);
  delay(200);
  motor_driver.setPWM(0, 0, 207);
  Serial.println("arming complete");
  receiver.setMinMax(minMax);
}

typedef struct {
  int leftMotorVal;
  int rightMotorVal;
} Steering;

Steering s_vals;
float rightTurnPercent;

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
    delay(20);
    stopHead();
  } else if (potVal > 1329 && potVal <= 1663) {
    stopHead();
  } else if (potVal > 1663 && potVal <= 1995) {
    digitalWrite(HEAD_INT_4, LOW);
    digitalWrite(HEAD_INT_3, HIGH);
    delay(20);
    stopHead();
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
  Serial.print('\n');
}

double scale(double num, double fromLow, double fromHigh, double toLow, double toHigh) {
  return toLow + ((toHigh - toLow) * ((num - fromLow) / (fromHigh - fromLow)));
}

double witch(double x) {
  return 1 / (x + 1);
}

int r_motorVal; // 207 - 411, end milli of pwm wave
int l_motorVal; // above
int throttle_pos; // 997-1995 end ms of pwm wave
int steering_pos; // 1000 - 1995
const int deadzone_l = 1400;
const int deadzone_r = 1600;
const int center = 1500;
double steering_perc = 1;
double proposed_speed_dif;

void displayMotorVals() {
  Serial.print("Left: ");
  Serial.print(l_motorVal);
  Serial.print("\tRight: " );
  Serial.println(r_motorVal);
  
}


void loop() {
  moveHead(receiver.getRaw(3));
  throttle_pos = receiver.getRaw(2);
  steering_pos = receiver.getRaw(1);
  r_motorVal = floor(scale(throttle_pos, 1000, 1995, 210, 411));
  l_motorVal = r_motorVal;
  if (steering_pos < deadzone_l) {  // steer left
    proposed_speed_dif = r_motorVal - scale(steering_pos, 1000, deadzone_l, 200, l_motorVal);
    steering_perc = witch(scale(r_motorVal, 200, 411, 0, 6));
    l_motorVal = r_motorVal - (proposed_speed_dif * steering_perc);
  }
  else if (steering_pos > deadzone_r) { // steer right
    proposed_speed_dif = l_motorVal - scale(steering_pos, 1995, deadzone_r, 200, l_motorVal);
    steering_perc = witch(scale(l_motorVal, 200, 411, 0, 6));
    r_motorVal = l_motorVal - floor(proposed_speed_dif * steering_perc);
  }

  // motor_driver.setPWM(0, 0, r_motorVal);
  // motor_driver.setPWM(1, 0, l_motorVal);
  // displayRadioChannels();
  displayMotorVals();
  delay(20);
}