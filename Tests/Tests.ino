#include <Zumo32U4.h>
#include <time.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4LineSensors lineSensors;
L3G gyro;

//The available tests are initialised here
void faceOpponent();
void stayInRing();
void charge();
uint16_t testSpeed = 200;

void setup() {
    //Uncomment the function you would like to test and ensure all
    //others are commented
//  faceOpponent();
//  stayInRing();
  charge();
}

void loop() {
}

void faceOpponent() {
  enum Direction {
  left,
  right,
  found
  };
  Direction scanDir = left;
  const uint8_t sensorThreshold = 3;
  proxSensors.initFrontSensor();
  while (1) {
    lcd.clear();
    //Scan in the direction the opponent was last seen
    if (scanDir == left) {
      lcd.print("Scan left");
      motors.setSpeeds(-testSpeed, testSpeed);
    } else if (scanDir == right) {
      lcd.print("Scan right");
      motors.setSpeeds(testSpeed, -testSpeed);
    } else {
      lcd.print("Found");
    }
  
    //Take readings from the proximity sensors
    proxSensors.read();
    uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
    uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
    //If the opponent has been spotted by either sensor,
      //attempt to face it head on
    if (leftValue >= sensorThreshold || rightValue >= sensorThreshold) {
      if (leftValue > rightValue) {
        scanDir = left;
      } else if (leftValue < rightValue) {
        scanDir = right;
      } else {
        scanDir = found;
        motors.setSpeeds(0, 0);
      }
    }
  }
}

void stayInRing() {
  lineSensors.initThreeSensors();
  unsigned int sensorValues[3];
  int timeLeft = 5;
  lcd.print(F("Calibrating"));
  while (timeLeft > 0) {
    lcd.gotoXY(0, 1);
    lcd.print(timeLeft);
    timeLeft--;
    lineSensors.calibrate();
    delay(1000);
  }
  while (1) {
    motors.setSpeeds(testSpeed, testSpeed);
    lineSensors.read(sensorValues, QTR_EMITTERS_ON);
    if (sensorValues[0] < 500 && sensorValues[2] < 500) {
      motors.setSpeeds(testSpeed, -testSpeed);
      while (sensorValues[0] < 500 && sensorValues[2] < 500) {
        lineSensors.read(sensorValues, QTR_EMITTERS_ON);
      }
      delay(500);
    } else if (sensorValues[0] < 500) {
      motors.setSpeeds(testSpeed, -testSpeed);
      while (sensorValues[0] < 500) {
        lineSensors.read(sensorValues, QTR_EMITTERS_ON);
      }
      delay(500);
    } else if (sensorValues[2] < 500) {
      motors.setSpeeds(-testSpeed, testSpeed);
      while (sensorValues[2] < 500) {
        lineSensors.read(sensorValues, QTR_EMITTERS_ON);
      }
      delay(500);
    }
  }
}

void charge() {
  const uint8_t sensorThreshold = 3;
  proxSensors.initFrontSensor();
  lineSensors.initThreeSensors();
  unsigned int sensorValues[3];
  int timeLeft = 5;
  lcd.print(F("Calibrating"));
  while (timeLeft > 0) {
    lcd.gotoXY(0, 1);
    lcd.print(timeLeft);
    timeLeft--;
    lineSensors.calibrate();
    delay(1000);
  }

  while (1) {
    int8_t diff;
    proxSensors.read();
    lineSensors.read(sensorValues, QTR_EMITTERS_ON);
    uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
    uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
    if (leftValue >= sensorThreshold || rightValue >= sensorThreshold) {
      lcd.clear();
      lcd.print("Charge");
      motors.setSpeeds(testSpeed, testSpeed);
      while (!(sensorValues[0] < 500) && !(sensorValues[2] < 500)) {
        lineSensors.read(sensorValues, QTR_EMITTERS_ON);
        proxSensors.read();
        leftValue = proxSensors.countsFrontWithLeftLeds();
        rightValue = proxSensors.countsFrontWithRightLeds();
        diff = leftValue - rightValue;
        if (diff > 0) {
          motors.setSpeeds(testSpeed/2, testSpeed);
        } else if (diff < 0) {
          motors.setSpeeds(testSpeed, testSpeed/2);
        } else {
          motors.setSpeeds(testSpeed, testSpeed);
        }
      }
      
      motors.setSpeeds(testSpeed, -testSpeed);
      while (sensorValues[0] < 500 || sensorValues[2] < 500) {
        lineSensors.read(sensorValues, QTR_EMITTERS_ON);
      }
      delay(500);
    }
    motors.setSpeeds(0, 0);
  }
}
