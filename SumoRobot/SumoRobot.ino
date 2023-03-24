#include <Zumo32U4.h>
#include <time.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4ButtonA buttonA;
Zumo32U4LineSensors lineSensors;
L3G gyro;
#include "search.h"

void setup() {
  srand((unsigned) time(0));
  //Prepare the countdown and initialise some of the sensors
  int timeLeft = 5;
  lineSensors.initThreeSensors();
  proxSensors.initFrontSensor();
  turnSensorSetup();
  turnSensorReset();
  lcd.clear();
  lcd.print(F("Press A"));
  buttonA.waitForButton();
  lcd.clear();
  //Count down from 5, calibrating the line sensor each second
  while (timeLeft > 0) {
    lcd.clear();
    lcd.print(timeLeft);
    timeLeft--;
    lineSensors.calibrate();
    delay(1000);
  }
  motors.setSpeeds(400, 400);
  delay(1000);
}

void loop() {
  //Go into the scan state
  scan();
}
