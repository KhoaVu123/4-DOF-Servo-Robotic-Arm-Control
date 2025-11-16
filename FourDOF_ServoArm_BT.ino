#include <Servo.h>
#include <SoftwareSerial.h>

// ---------------- PIN CONFIGURATION ----------------
const int SERVO1_PIN = 3;   // Base servo
const int SERVO2_PIN = 5;   // Joint 2 servo
const int SERVO3_PIN = 6;   // Joint 3 servo
const int SERVO4_PIN = 9;   // Gripper / end-effector servo

const int BT_RX_PIN = 10;   // Arduino RX from Bluetooth (TX of HC-05)
const int BT_TX_PIN = 11;   // Arduino TX to Bluetooth (RX of HC-05)

// ---------------- OBJECT DECLARATION ----------------
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

SoftwareSerial BT(BT_RX_PIN, BT_TX_PIN); // RX, TX

String inputString = "";   // String received from Bluetooth

// Safe angle limits for servos
const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;

// ---------------- HELPER FUNCTIONS ----------------

// Clamp angle to the range MIN_ANGLE..MAX_ANGLE
int clampAngle(int angle) {
  if (angle < MIN_ANGLE) return MIN_ANGLE;
  if (angle > MAX_ANGLE) return MAX_ANGLE;
  return angle;
}

// Handle command string in the format "a,b,c,d"
void processCommand(const String &cmd) {
  // Find comma positions
  int firstComma  = cmd.indexOf(',');
  int secondComma = cmd.indexOf(',', firstComma + 1);
  int thirdComma  = cmd.indexOf(',', secondComma + 1);

  // Ignore if there are not exactly 3 commas
  if (firstComma == -1 || secondComma == -1 || thirdComma == -1) {
    Serial.println("Invalid command format! Use: angle1,angle2,angle3,angle4");
    return;
  }

  // Split into parts
  String s1 = cmd.substring(0, firstComma);
  String s2 = cmd.substring(firstComma + 1, secondComma);
  String s3 = cmd.substring(secondComma + 1, thirdComma);
  String s4 = cmd.substring(thirdComma + 1);

  int angle1 = clampAngle(s1.toInt());
  int angle2 = clampAngle(s2.toInt());
  int angle3 = clampAngle(s3.toInt());
  int angle4 = clampAngle(s4.toInt());

  // Print to Serial for debugging
  Serial.print("Received: ");
  Serial.print(angle1); Serial.print(", ");
  Serial.print(angle2); Serial.print(", ");
  Serial.print(angle3); Serial.print(", ");
  Serial.println(angle4);

  // Control servos
  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
  servo4.write(angle4);

  // Send feedback through Bluetooth
  BT.print("OK: ");
  BT.print(angle1); BT.print(",");
  BT.print(angle2); BT.print(",");
  BT.print(angle3); BT.print(",");
  BT.println(angle4);
}

// ---------------- SETUP ----------------
void setup() {
  // Attach servos to pins
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);

  // Initial angles (adjust to match your mechanical zero positions)
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);

  // PC Serial for debugging
  Serial.begin(9600);
  Serial.println("Starting 4-DOF servo arm control via Bluetooth");
  Serial.println("Send command: angle1,angle2,angle3,angle4 (e.g. 90,45,120,60)");

  // Bluetooth Serial
  BT.begin(9600); // default baud rate of HC-05 is 9600
  BT.println("Bluetooth connected. Send command: angle1,angle2,angle3,angle4");
}

// ---------------- LOOP ----------------
void loop() {
  // Read data from Bluetooth
  while (BT.available() > 0) {
    char c = BT.read();

    // Ignore carriage return '\r'
    if (c == '\r') {
      continue;
    }

    // When we see '\n', process the full string
    if (c == '\n') {
      if (inputString.length() > 0) {
        processCommand(inputString);
        inputString = "";
      }
    } else {
      // Append character to buffer if not too long
      if (inputString.length() < 50) {
        inputString += c;
      }
    }
  }

  // (Optional) You can add demo behavior here if no command is received for a while...
}
