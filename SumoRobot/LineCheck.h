#include "TurnSensor.h"
int32_t getAngle();
void turn(int dir, int amount);

bool lineCheck() {
  unsigned int sensorValues[3];
  bool turning;
  int amount, turnDir, turnAmount;
  turning = false;

  //Take readings from the line sensors
  lineSensors.read(sensorValues, QTR_EMITTERS_ON);
  //If a white line has been detected, reverse until no longer on it
    //and turn a random amount within a range dependent on where the line is
  if (sensorValues[0] < 500 && sensorValues[2] < 500) {
    turnDir = 1;
    amount = rand() % 201 + 160;
    if (amount > 180) {
      amount -= 180;
      turnDir = -1;
    }
    turning = true;
  } else if (sensorValues[0] < 500) {
    amount = rand() % 111 + 70;
    turnDir = 1;
    turning = true;
  } else if (sensorValues[2] < 500) {
    amount = rand() % 111 + 70;
    turnDir = -1;
    turning = true;
  }
  if (turning) {
    while (sensorValues[0] < 500 || sensorValues[2] < 500) {
      motors.setSpeeds(-400, -400);
      delay(500);
      lineSensors.read(sensorValues);
    }
    turn(turnDir, amount);
    return true;
  }
  return false;
}

void turn(int dir, int amount) {
  //Turn while using the gyroscope to determine the angle of rotation
  int32_t angle = 0;
  turnSensorReset();
  motors.setSpeeds(dir * 400, -dir * 400);
  while (angle < amount) {
    turnSensorUpdate();
    angle = abs(getAngle());
  }
}
