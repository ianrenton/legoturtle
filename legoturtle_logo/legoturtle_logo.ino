// Lego Turtle "Logo" program
// Drives a tracked Lego Mindstorms robot according to a simple Logo
// program specified at programming time.
// Only the four basic commands are supported: fd, bk, lt, rt.
// Distances are in cm and angles in degrees (approximately)
// e.g. the following program creates a 1m-sided square:
// fd 100\nrt 90\nfd 100\nrt 90\nfd 100\nrt 90\nfd 100
// Press a flipper to start the program (when LED is flashing).
// Pressing a flipper again, an invalid command, or reaching the 
// end will stop the program.

// Logo program
//String program = "lt 90\nrt 180\nlt 120\n";
//String program = "fd 100\nrt 90\nfd 100\nrt 90\nfd 100\nrt 90\nfd 100\n";
String program = "fd 80\nrt 120\nfd 80\nrt 120\nfd 80\nrt 120\n";

// Motion controls
long DRIVE_MILLISEC_PER_CM = 100;
long TURN_MILLISEC_PER_DEGREE = 20;
long TURN_MILLISEC_CONSTANT = 3000;

// Port numbers
int LEFT_FLIPPER = 2;
int RIGHT_FLIPPER = 3;
int MOTOR_LEFT_FWD = 9;
int MOTOR_LEFT_REV = 10;
int MOTOR_RIGHT_FWD = 11;
int MOTOR_RIGHT_REV = 12;
int LED = 13;

// Internal vars
boolean keepRunning;

void setup() {
  // Serial setup
  Serial.begin(9600);
  while (!Serial) { ; }
  
  // DIO direction setting
  pinMode(LEFT_FLIPPER, INPUT);
  pinMode(RIGHT_FLIPPER, INPUT);
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_REV, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_REV, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  
  // Wait for flipper push to start program running
  Serial.println("Press a flipper to start the program.");
  boolean start = false;
  boolean ledToggle = false;
  while(!start) {
    delay(100);
    boolean hitLeft = (digitalRead(LEFT_FLIPPER) == HIGH);
    boolean hitRight = (digitalRead(RIGHT_FLIPPER) == HIGH);
    start = hitLeft || hitRight;
    digitalWrite(LED, ledToggle);
    ledToggle = !ledToggle;
  }
  
  // Now parse and run the program until it ends naturally or
  // a flipper is pressed to stop.
  // Turn the light on to indicate that the program is running.
  digitalWrite(LED, HIGH);
  Serial.println("Starting program.");
  // Delay for user to get clear
  delay(1000);
  int prevDelimiterPos = -1;
  int newDelimiterPos;
  keepRunning = true;
  while (keepRunning) {
    // Check if we have reached the end
    if (prevDelimiterPos + 1 >= program.length()) {
      Serial.println("Reached end of program.");
      stopProgram();
    }
    else
    {
      // Parse the next command from the Logo program
      newDelimiterPos = program.indexOf('\n', prevDelimiterPos + 1);
      String newCommand = program.substring(prevDelimiterPos + 1, newDelimiterPos);
      String action = newCommand.substring(0,2);
      long number = newCommand.substring(3,newCommand.length()).toInt();
      Serial.println("Received command: " + newCommand);
      if (number <= 0) {
        Serial.println("Invalid command: " + newCommand);
        stopProgram();
      }
      else
      {
        if (action == "fd") {
          Serial.println("Driving forwards " + String(number));
          driveForwards(number);
        } else if (action == "bk") {
          Serial.println("Driving backwards " + String(number));
          driveBackwards(number);
        } else if (action == "lt") {
          Serial.println("Turning left " + String(number));
          turnLeft(number);
        } else if (action == "rt") {
          Serial.println("Turning right " + String(number));
          turnRight(number);
        } else {
          Serial.println("Invalid command: " + newCommand);
          stopProgram();
        }
      }
      prevDelimiterPos = newDelimiterPos;
    }
  }
}

// Set the motors to drive forwards for a certain distance (cm)
void driveForwards(long cm) {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_REV, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_REV, LOW);
  delayUnlessInterrupted(cm*DRIVE_MILLISEC_PER_CM);
}

// Set the motors to drive backwards for a certain distance (cm)
void driveBackwards(long cm) {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_REV, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_REV, HIGH);
  delayUnlessInterrupted(cm*DRIVE_MILLISEC_PER_CM);
}

// Set the motors to turn left for a certain angle (deg)
void turnLeft(long angle) {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_REV, HIGH);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_REV, LOW);
  delayUnlessInterrupted(angle*TURN_MILLISEC_PER_DEGREE+TURN_MILLISEC_CONSTANT);
}

// Set the motors to turn right for a certain angle (deg)
void turnRight(long angle) {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_REV, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_REV, HIGH);
  delayUnlessInterrupted(angle*TURN_MILLISEC_PER_DEGREE+TURN_MILLISEC_CONSTANT);
}

// Set the motors to stop (instantaneous operation)
void stopDriving() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_REV, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_REV, LOW);
}

// Equivalent to delay(n) but with a check to stop the program
// if a flipper has been hit.
void delayUnlessInterrupted(long msec) {
  for (long i=0; i<msec; i=i+100) {
    delay(msec/100);
    boolean hitLeft = (digitalRead(LEFT_FLIPPER) == HIGH);
    boolean hitRight = (digitalRead(RIGHT_FLIPPER) == HIGH);
    if (hitLeft || hitRight) {
      stopProgram();
      break;
    }
  }
}

// Stops the program and turns the Arduino light off so the user
// knows it's finished.
void stopProgram() {
  Serial.println("Stopping program.");
  keepRunning = false;
  stopDriving();
  digitalWrite(LED, LOW);
  delay(1000);
}

