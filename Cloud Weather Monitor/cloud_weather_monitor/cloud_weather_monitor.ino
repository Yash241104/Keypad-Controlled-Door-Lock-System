#include <WiFi.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include "ThingSpeak.h" // Include the ThingSpeak library

// --- WiFi Configuration ---
const char* ssid = "OnePlus Nord CE3 5G";         // Your WiFi network SSID
const char* password = "yashyash"; // Your WiFi network password

// --- ThingSpeak Configuration ---
unsigned long myChannelNumber = 3014054; // Replace with your ThingSpeak Channel ID
const char * myWriteAPIKey = "8T2IKTSC7ZABDJSK"; // Replace with your ThingSpeak Write API Key

// --- DHT11 Sensor Configuration ---
#define DHTPIN 4       // DHT11 Data pin connected to ESP32 GPIO 4
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// --- WiFiClient for ThingSpeak ---
WiFiClient  client;

// --- Update interval (in milliseconds) ---
unsigned long lastUpdateTime = 0;
const long updateInterval = 2000; // Update every 20 seconds (ThingSpeak free tier limit is often 15 seconds)

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin(); // Initialize DHT sensor
  ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop() {
  // Check if it's time to update ThingSpeak
  if (millis() - lastUpdateTime > updateInterval) {
    // Read sensor data
    float humidity = dht.readHumidity();
    float temperatureC = dht.readTemperature(); // Celsius
    float temperatureF = dht.readTemperature(true); // Fahrenheit

    // Check if any reads failed
    if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
      Serial.println("Failed to read from DHT sensor!");
      // Optionally handle this error, e.g., don't send data to ThingSpeak
    } else {
      Serial.print("Temperature: ");
      Serial.print(temperatureC);
      Serial.print(" *C / ");
      Serial.print(temperatureF);
      Serial.print(" *F\t");
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      // Set the fields with the sensor readings
      ThingSpeak.setField(1, temperatureC); // Field 1 for Celsius
      ThingSpeak.setField(2, humidity);     // Field 2 for Humidity

      // You could set more fields if you added them in ThingSpeak
      // ThingSpeak.setField(3, temperatureF); // Example: Field 3 for Fahrenheit

      // Write the data to ThingSpeak
      int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

      if (httpCode == 200) {
        Serial.println("Channel update successful.");
      } else {
        Serial.print("Problem updating channel. HTTP error code ");
        Serial.println(httpCode);
      }
    }
    lastUpdateTime = millis(); // Update the last update time
  }
}