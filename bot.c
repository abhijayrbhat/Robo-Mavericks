#include <SPI.h>
#include <LoRa.h>

// Replace with your LoRa settings
const int csPin = SS; // LoRa chip select pin (usually SS)
const int resetPin = Rst; // LoRa reset pin (optional)
const int freq = 915E6; // LoRa frequency
const int power = -4; // LoRa TX power (in dBm)

// Define joystick pins (adjust based on your joystick setup)
const int xAxisPin = A0;
const int yAxisPin = A1;
const int buttonPin = 2; // Optional button for confirmation


const int threshold = 50; 

const char* forwardCmd = "F";
const char* backwardCmd = "B";
const char* leftCmd = "L";
const char* rightCmd = "R";
int counter = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender with Joystick Control");
  // Initialize LoRa communication
  if (!LoRa.begin(freq, csPin, resetPin, power)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Assuming button has internal pull-up
}
void loop() {
  int xVal = analogRead(xAxisPin);
  int yVal = analogRead(yAxisPin);
  bool buttonPressed = !digitalRead(buttonPin); // Active low button
  String command = "";
  // Check for forward/backward movement
  if (yVal > 1023 - threshold) {
    command = forwardCmd;  // Forward
  } else if (yVal < threshold) {
    command = backwardCmd; // Backward
  }
  // Check for left/right turn (if no forward/backward movement)
  if (command == "" && xVal > 1023 - threshold) {
    command = leftCmd;  // Left turn
  } else if (command == "" && xVal < threshold) {
    command = rightCmd; // Right turn
  }
  if (command != "" && buttonPressed) { // Send command only if a valid command exists and button is pressed
    Serial.print("Sending command: ");
    Serial.println(command);
    LoRa.beginPacket();
    LoRa.print(command);
    LoRa.endPacket();
    counter++;
  }
  delay(100); // Adjust delay based on movement sensitivity preference
}