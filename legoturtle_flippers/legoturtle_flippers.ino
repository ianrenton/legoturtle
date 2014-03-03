// Lego Turtle "Flippers" program
// Drives a tracked Lego Mindstorms robot forwards, backing up and turning when
// a wall is reached (detected by flippers on the front of the robot).

// Port numbers
int LEFT_FLIPPER = 2;
int RIGHT_FLIPPER = 3;
int MOTOR_LEFT_FWD = 9;
int MOTOR_RIGHT_FWD = 10;
int MOTOR_LEFT_REV = 11;
int MOTOR_RIGHT_REV = 12;

void setup() {
  // DIO direction setting
  pinMode(LEFT_FLIPPER, INPUT);
  pinMode(RIGHT_FLIPPER, INPUT);
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_REV, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_REV, OUTPUT);
}

void loop() {
  boolean hitLeft = false;
  boolean hitRight = false;
  
  while (true) {
    // Keep driving until you hit something
    while (!(hitLeft || hitRight)) {
      driveForwards();
      hitLeft = (digitalRead(LEFT_FLIPPER) == HIGH);
      hitRight = (digitalRead(RIGHT_FLIPPER) == HIGH);
    }
    // When something is hit, back up and turn
    stopDriving();
    backUp();
    if (hitLeft && hitRight) {
      turnAround();
    } else if (hitLeft) {
      turnLeft();
    } else if (hitRight) {
      turnRight();
    }
    // Reset these so we go into the main loop again.
    hitLeft = false;
    hitRight = false;
  }
}

// Set the motors to drive forwards (instantaneous operation)
void driveForwards() {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_REV, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_REV, LOW);
}

// Set the motors to stop (instantaneous operation)
void stopDriving() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_REV, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_REV, LOW);
}

// Set the motors to drive backwards for two seconds, then stop
void backUp() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_REV, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_REV, HIGH);
  delay(2000);
  stopDriving();
}

// Set the motors to turn left for one second, then stop
void turnLeft() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_REV, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_REV, LOW);
  delay(1000);
  stopDriving();
}

// Set the motors to turn right for one second, then stop
void turnRight() {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_REV, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_REV, HIGH);
  delay(1000);
  stopDriving();
}

// Set the motors to turn left for two seconds, then stop
void turnAround() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_REV, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_REV, LOW);
  delay(2000);
  stopDriving();
}
