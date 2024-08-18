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

// https://medium.com/@werneckpaiva/how-to-read-rc-receiver-signal-with-arduino-54e0447f6c3f
int readChannel(int channelInput, int minLimit, int maxLimit){
  return pulseIn(channelInput, HIGH, 30000);
}

// Read the channel and return a boolean value
// bool readSwitch(byte channelInput, bool defaultValue){
//   int intDefaultValue = (defaultValue)? 100: 0;
//   int ch = readChannel(channelInput, 0, 100, intDefaultValue);
//   return (ch > 50);
// }


void setup() {
  Serial.begin(9600);
  for (auto CH : {CH1, CH2, CH3, CH4, CH5, CH6}) {
    pinMode(CH, INPUT);
  }
  left.attach(2, 1000, 2000);
  right.attach(3, 1000, 2000);
  calibrateESC();
  Serial.println("arming complete");

}
int motorVal;
void loop() {
  
  // Serial.print("Ch1: ");
  // Serial.print(readChannel(CH1, -100, 100, 0));
  // Serial.print(" Ch2: ");
  // Serial.print(readChannel(CH2, -100, 100, 0));
  // Serial.print(" Ch3: ");
  Serial.println(readChannel(CH3, 0, 180));
  // Serial.print(" Ch4: "); 
  // Serial.println(readSwitch(CH5, false));
  // Serial.print(readChannel(CH4, -100, 100, 0));
  // Serial.print(" Ch5: ");
  // Serial.println(readSwitch(CH5, false));
  
  //left.write(readChannel(CH3, 0, 180));
  // right.write(motorVal);
}