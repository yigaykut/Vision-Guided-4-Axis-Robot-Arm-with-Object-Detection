# Vision-Guided 4-Axis Robot Arm with Object Detection

This repository contains the code for a 4-axis robotic arm that uses a local Qwen Vision-Language Model to calculate joint angles for grabbing objects and utilizes an ultrasonic distance sensor for close-range grip triggers.

## Project Structure

* **`arm.ino`**: Arduino sketch managing the 4 servos and reading the HC-SR04 distance sensor. It features a basic software-based easing loop to keep joint movements steady instead of snapping directly to targets.
* **`bridge_arm.py`**: Python script that captures the camera feed, hits the local Qwen endpoint with a visual prompt, parses coordinates into joint angles, and sends them via serial.

## Hardware Setup

### Components
* Arduino Uno or Nano
* 4x SG90 or MG996R Servo Motors
* 1x HC-SR04 Ultrasonic Distance Sensor
* 1x USB Webcam or Camera Module
* External 5V Power Supply for the servos

### Wiring Layout

#### Servos to Arduino
* **Base Servo:** Pin 3
* **Shoulder Servo:** Pin 5
* **Elbow Servo:** Pin 6
* **Gripper Servo:** Pin 9

#### HC-SR04 Sensor to Arduino
* **VCC:** 5V
* **GND:** GND
* **Trigger:** Pin 7
* **Echo:** Pin 8


## How It Works

1. The Python script sends the live camera frame to the Qwen VLM. The prompt instructs the model to return spatial coordinates translated directly into degrees for the base, shoulder, and elbow joints.
2. Python packages these values into a string starting with a `#` marker, such as `#90 45 60 20`, and sends it over the serial port.
3. The Arduino updates the arm position continuously. However, if the HC-SR04 ultrasonic sensor detects an object closer than 8 centimeters directly inside the gripper path, it automatically overrides the current gripper state and commands it to clamp down immediately.

## Start

1. Upload `arm.ino` to your board using the Arduino IDE.
2. Open `bridge_arm.py` and change the serial port string to match your system environment, like `COM4` on Windows or `/dev/ttyUSB0` on Linux.
3. Install dependencies and boot the bridge loop:
   ```bash
   pip install opencv-python pyserial requests
   python bridge_arm.py
