#include <Keypad.h>
#include <ESP32Servo.h>

// --- Keypad Configuration ---
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {26, 25, 33, 32};
byte colPins[COLS] = {19, 18, 5, 17};

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- Servo Motor Configuration ---
#define SERVO_PIN 13
Servo doorServo;

// Adjust these angles for your specific servo's locked and unlocked positions
const int LOCKED_ANGLE = 0;
const int UNLOCKED_ANGLE = 90;

const int UNLOCK_DURATION_MS = 3000; // Door unlocks for 3 seconds
unsigned long unlockTimer = 0;
bool isLocked = true;

// --- Status Indicator Configuration ---
const int UNLOCK_LED = 16;

// --- Access Code Configuration ---
const String SECRET_CODE = "1234";
String enteredCode = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Keypad Door Lock System - Ready!");
  Serial.println("Secret Code: " + SECRET_CODE);

  pinMode(UNLOCK_LED, OUTPUT);
  digitalWrite(UNLOCK_LED, LOW); // LED off (locked state)

  doorServo.attach(SERVO_PIN);
  doorServo.write(LOCKED_ANGLE); // Set servo to locked position
  Serial.println("Door is initially LOCKED.");
}

void loop() {
  char key_pressed = customKeypad.getKey();

  if (key_pressed) {
    Serial.print("Key Pressed: ");
    Serial.println(key_pressed);

    // Handle special keys: '*' to clear, '#' to attempt unlock
    if (key_pressed == '*') {
      enteredCode = "";
      Serial.println("Code Cleared.");
    } else if (key_pressed == '#') {
      Serial.print("Attempting to unlock with: ");
      Serial.println(enteredCode);

      if (enteredCode == SECRET_CODE) {
        Serial.println("Code Correct! Unlocking...");
        unlockDoor();
        unlockTimer = millis(); // Record unlock time
      } else {
        Serial.println("Incorrect Code!");
      }
      enteredCode = ""; // Clear code after attempt
    } else {
      // Append regular digits/characters to entered code
      if (enteredCode.length() < SECRET_CODE.length() + 5) {
         enteredCode += key_pressed;
         Serial.print("Current Entered Code: ");
         Serial.println(enteredCode);
      }
    }
  }

  // Auto-lock mechanism: if unlocked and duration elapsed
  if (!isLocked && millis() - unlockTimer >= UNLOCK_DURATION_MS) {
    Serial.println("Unlock duration elapsed. Auto-locking door...");
    lockDoor();
  }
}

void unlockDoor() {
  if (isLocked) { // Only unlock if currently locked
    doorServo.write(UNLOCKED_ANGLE);
    digitalWrite(UNLOCK_LED, HIGH);
    isLocked = false;
    Serial.println("Door is UNLOCKED!");
  }
}

void lockDoor() {
  if (!isLocked) { // Only lock if currently unlocked
    doorServo.write(LOCKED_ANGLE);
    digitalWrite(UNLOCK_LED, LOW);
    isLocked = true;
    Serial.println("Door is LOCKED.");
  }
}
