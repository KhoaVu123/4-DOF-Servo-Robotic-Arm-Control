# 4-DOF Servo Robotic Arm Control via Bluetooth (HC-05 + Arduino)

This sketch controls a 4-DOF (4 servo) robotic arm using an Arduino and an HC-05 Bluetooth module.
The arm is controlled by sending angle commands over Bluetooth in the format:

    angle1,angle2,angle3,angle4

For example:

    90,45,120,60

Each value is a servo angle in degrees (0–180).

------------------------------------------------------------
## Hardware Requirements
------------------------------------------------------------

- Arduino (Uno, Nano, etc.)
- 4x Servo motors
- HC-05 Bluetooth module
- External power supply for servos (recommended)
- Jumper wires and breadboard

------------------------------------------------------------
## Pin Configuration
------------------------------------------------------------

In the code:

    // Servo pins
    const int SERVO1_PIN = 3;   // Base servo
    const int SERVO2_PIN = 5;   // Joint 2 servo
    const int SERVO3_PIN = 6;   // Joint 3 servo
    const int SERVO4_PIN = 9;   // Gripper / end-effector servo

    // Bluetooth pins (SoftwareSerial)
    const int BT_RX_PIN = 10;   // Arduino RX (connect to HC-05 TX)
    const int BT_TX_PIN = 11;   // Arduino TX (connect to HC-05 RX)

Wiring:

- Servos
  - Signal pins: 3, 5, 6, 9
  - Servo VCC: external 5V supply (do NOT draw all servo power from Arduino 5V if they are high torque)
  - Servo GND: connect to external supply GND AND Arduino GND (common ground)

- HC-05 Bluetooth
  - HC-05 TX -> Arduino pin 10 (BT_RX_PIN)
  - HC-05 RX -> Arduino pin 11 (BT_TX_PIN)
    (Use a voltage divider from Arduino TX (5V) -> HC-05 RX (3.3V) if needed)
  - HC-05 VCC -> 5V
  - HC-05 GND -> GND

------------------------------------------------------------
## Software Setup
------------------------------------------------------------

1. Open the Arduino IDE.
2. Make sure these libraries are available (usually built-in):
   - Servo.h
   - SoftwareSerial.h
3. Copy the Arduino sketch into the IDE.
4. Select the correct board and COM port.
5. Upload the code to the Arduino.

------------------------------------------------------------
## How It Works
------------------------------------------------------------

- The Arduino listens on a software serial port (BT) at 9600 baud for Bluetooth commands.
- Commands must be in the form:

      angle1,angle2,angle3,angle4

  Example:

      0,90,135,180

- The code:
  1. Reads characters from Bluetooth until it sees a newline '\n'.
  2. Splits the received string by commas into 4 parts.
  3. Converts each part to an integer and clamps each angle to the safe range [0, 180].
  4. Writes the angles to servo1, servo2, servo3, and servo4.
  5. Sends back a confirmation message like:

         OK: 0,90,135,180

- All received and parsed angles are also printed to the hardware Serial monitor (USB) for debugging.

------------------------------------------------------------
## Using a Bluetooth App
------------------------------------------------------------

You can use any serial Bluetooth terminal app on your phone (for example: "Serial Bluetooth Terminal"):

1. Pair your phone with the HC-05 (default password usually 1234 or 0000).
2. Open the serial Bluetooth app and connect to the HC-05.
3. Set:
   - Baud rate: 9600
   - Line ending: newline ('\n') or "CR+LF", depending on the app.
4. Type commands such as:

       90,90,90,90

   and send.

The arm will move each servo to the specified angle, and the Arduino will respond with "OK: ...".

------------------------------------------------------------
## Safety Notes
------------------------------------------------------------

- Make sure the mechanical joints of the arm cannot be forced beyond their physical limits.
- The code clamps angles to 0–180 degrees:

      const int MIN_ANGLE = 0;
      const int MAX_ANGLE = 180;

  Adjust these if your servos or arm geometry require a smaller safe range.

- Always use an adequate external power supply for the servos to avoid resetting the Arduino.

------------------------------------------------------------
## Customization
------------------------------------------------------------

- Change the initial servo positions in setup():

      servo1.write(90);
      servo2.write(90);
      servo3.write(90);
      servo4.write(90);

- Edit the serial/Bluetooth messages to your preferred language (English / Vietnamese).
- Add more logic in loop() (for example: timeouts, demo motions, preset poses) if no command is received for a period.

------------------------------------------------------------
## File Naming
------------------------------------------------------------

Suggested Arduino sketch filename:

    FourDOF_ServoArm_BT.ino

(Arduino IDE requires the folder name to match the .ino filename.)
