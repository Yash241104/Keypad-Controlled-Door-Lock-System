# IoT Projects

This repository showcases a collection of Internet of Things (IoT) projects.

---

## Projects

Here's an overview of the IoT projects currently available in this repository:

### 1. Keypad-Based Lock System with Servo Motor and LED Blink

* **Description:** This project implements a simple yet effective access control system using a keypad. Users can enter a pre-defined PIN to control a servo motor (acting as a lock/unlock mechanism) and receive visual feedback via an LED.
* **Key Features:**
    * PIN-based authentication.
    * Servo motor control for lock/unlock action.
    * LED indicator for system status (e.g., locked, unlocked, wrong PIN).
* **Components/Technologies:**
    * Microcontroller (e.g., Arduino, ESP32)
    * 4x4 Membrane Keypad
    * Servo Motor
    * LEDs
* **Location:** `keypad-lock-system/` (or similar folder name you'll use)

### 2. Cloud-Based Weather Monitor (ESP32 & ThingSpeak)

* **Description:** A smart weather station that measures ambient temperature and humidity using a DHT11 sensor. The data is then wirelessly transmitted from an ESP32 microcontroller to the ThingSpeak cloud platform for real-time visualization, logging, and remote monitoring.
* **Key Features:**
    * Real-time temperature ($\deg$C and $\deg$F) and humidity (%) readings.
    * Data uploaded to ThingSpeak for cloud storage and graphing.
    * Remote access to weather data from anywhere via the internet.
* **Components/Technologies:**
    * ESP32 Development Board
    * DHT11 Temperature and Humidity Sensor
    * Wi-Fi Connectivity
    * ThingSpeak IoT Platform
* **Location:** `cloud-weather-monitor/` (or similar folder name you'll use)
