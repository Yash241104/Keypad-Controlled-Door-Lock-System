#include <Keypad.h>    // Keypad library for reading input
#include <ESP32Servo.h> // Servo library for controlling the servo motor

// --- Keypad Configuration ---
const byte ROWS = 4; // Number of rows in your keypad
const byte COLS = 4; // Number of columns in your keypad

// Define the character map for your 4x4 keypad
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Define the ESP32 GPIO pins connected to the ROWS of the keypad
// IMPORTANT: Ensure these match your physical wiring!
byte rowPins[ROWS] = {26, 25, 33, 32}; // Example GPIOs for rows (R1-R4)

// Define the ESP32 GPIO pins connected to the COLUMNS of the keypad
// IMPORTANT: Ensure these match your physical wiring!
byte colPins[COLS] = {19, 18, 5, 17};  // Example GPIOs for columns (C1-C4)

// Create the Keypad object
Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- Servo Motor Configuration ---
#define SERVO_PIN 13       // ESP32 GPIO pin connected to the servo's signal wire
Servo doorServo;           // Servo object

// Define the angles for locked and unlocked states (adjust these for your servo!)
// You will likely need to experiment with your specific servo to find the best angles.
const int LOCKED_ANGLE = 0;   // Example: Servo at 0 degrees for locked
const int UNLOCKED_ANGLE = 90; // Example: Servo at 90 degrees for unlocked

const int UNLOCK_DURATION_MS = 3000; // Duration the door stays "unlocked" (3 seconds)
unsigned long unlockTimer = 0;       // Variable to store the time when unlocked
bool isLocked = true;              // Flag to track the lock state

// --- Status Indicator Configuration ---
const int UNLOCK_LED = 16;  // ESP32 GPIO pin for the "unlocked" LED

// --- Access Code Configuration ---
const String SECRET_CODE = "1234"; // Your secret 4-digit code (e.g., "1234")
String enteredCode = "";           // String to store characters entered by the user

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  Serial.println("Keypad Door Lock System - Ready!");
  Serial.println("Secret Code: " + SECRET_CODE);

  // Initialize LED pin as output and ensure it's off initially
  pinMode(UNLOCK_LED, OUTPUT);
  digitalWrite(UNLOCK_LED, LOW); // LED off (locked state)

  // Attach the servo to its pin and set it to the initial locked position
  doorServo.attach(SERVO_PIN);
  doorServo.write(LOCKED_ANGLE); // Set servo to locked position
  Serial.println("Door is initially LOCKED.");
}

void loop() {
  // Get a key from the keypad
  char key_pressed = customKeypad.getKey();

  // If a key was pressed
  if (key_pressed) {
    Serial.print("Key Pressed: ");
    Serial.println(key_pressed);

    // Handle special keys: '*' to clear, '#' to attempt unlock
    if (key_pressed == '*') {
      enteredCode = ""; // Clear the entered code
      Serial.println("Code Cleared.");
    } else if (key_pressed == '#') {
      Serial.print("Attempting to unlock with: ");
      Serial.println(enteredCode);

      // Check if the entered code matches the secret code
      if (enteredCode == SECRET_CODE) {
        Serial.println("Code Correct! Unlocking...");
        unlockDoor();           // Call function to unlock
        unlockTimer = millis(); // Record the current time for the unlock duration
      } else {
        Serial.println("Incorrect Code!");
        // Optional: Add some feedback for incorrect code here (e.g., a short LED flash, a small delay)
      }
      enteredCode = ""; // Clear the code after an attempt (correct or incorrect)
    } else {
      // If it's a regular digit/character, add it to the entered code string
      // Limit the length to prevent excessively long codes
      if (enteredCode.length() < SECRET_CODE.length() + 5) { // Allow slightly longer for user input errors
         enteredCode += key_pressed;
         Serial.print("Current Entered Code: ");
         Serial.println(enteredCode);
      }
    }
  }

  // --- Auto-Lock Mechanism ---
  // If the door is currently unlocked AND the unlock duration has passed
  if (!isLocked && millis() - unlockTimer >= UNLOCK_DURATION_MS) {
    Serial.println("Unlock duration elapsed. Auto-locking door...");
    lockDoor(); // Call function to lock the door
  }
}

// Function to handle unlocking the door
void unlockDoor() {
  if (isLocked) { // Only unlock if currently locked
    doorServo.write(UNLOCKED_ANGLE); // Move servo to unlocked position
    digitalWrite(UNLOCK_LED, HIGH);   // Turn on the unlock LED
    isLocked = false;                 // Update lock state
    Serial.println("Door is UNLOCKED!");
  }
}

// Function to handle locking the door
void lockDoor() {
  if (!isLocked) { // Only lock if currently unlocked
    doorServo.write(LOCKED_ANGLE);  // Move servo to locked position
    digitalWrite(UNLOCK_LED, LOW);   // Turn off the unlock LED
    isLocked = true;                 // Update lock state
    Serial.println("Door is LOCKED.");
  }
}




