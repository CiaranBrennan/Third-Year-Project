#include "LineCheck.h"
uint16_t chaseSpeed = 400;
uint16_t veerSpeedHi = 300;
uint16_t veerSpeedLo = 100;
const uint8_t sensorThreshold = 1;
bool targetFound = false;
int8_t diff;

enum Direction {
  left,
  right
};

//Set the default direction for the robot to scan
Direction scanDir = left;

void charge();

void scan() {
  lcd.clear();
  //Scan in the direction the opponent was last seen
  if (scanDir == left) {
    lcd.print("Scan left");
    motors.setSpeeds(-chaseSpeed, chaseSpeed);
  } else {
    lcd.print("Scan right");
    motors.setSpeeds(chaseSpeed, -chaseSpeed);
  }

  //Take readings from the proximity sensors
  proxSensors.read();
  uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
  uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
  //If the opponent has been spotted, go to the charge state
  if (leftValue >= sensorThreshold || rightValue >= sensorThreshold) {
    lcd.clear();
    lcd.print("Charge");
    charge();
  }
}

void charge() {
  int8_t diff;
  //Keep driving until you reach the edge of the ring
  while (!lineCheck()) {
    proxSensors.read();
    uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
    uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
    diff = leftValue - rightValue;
    //Adjust direction to follow the opponent's movements if needed
    if (diff > 0) {
      motors.setSpeeds(veerSpeedLo, veerSpeedHi);
      scanDir = left;
    } else if (diff < 0) {
      motors.setSpeeds(veerSpeedHi, veerSpeedLo);
      scanDir = right;
    } else {
      motors.setSpeeds(chaseSpeed, chaseSpeed);
    }
  }
}
