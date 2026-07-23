// 1. Blynk Template Settings (MUST BE AT THE VERY TOP)
#define BLYNK_TEMPLATE_ID "TMPL31cK24_SR"
#define BLYNK_TEMPLATE_NAME "smoke sensor"
#define BLYNK_AUTH_TOKEN "tZekZP_2dbVD2k3mmGEa_NbnoalrlX-F"

// 2. Include Libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// 3. Wi-Fi Credentials (Fill in your home Wi-Fi details here)
const char* ssid     = "Saikiran4Ggg";
const char* password = "Sai@787027";

// 4. Pin Definitions
#define SMOKE_SENSOR_PIN 34  // Analog pin GPIO 34 on ESP32
#define BUZZER_PIN       25  // Local physical buzzer pin (Optional)

// 5. Threshold Configuration
const int SMOKE_THRESHOLD = 1200; // Adjust this based on your room's clean air readings
BlynkTimer timer;

// Function to read sensor and send data to Blynk
void checkSmokeLevel() {
  // Read analog value from the sensor (ESP32 ADC ranges from 0 to 4095)
  int smokeLevel = analogRead(SMOKE_SENSOR_PIN);
  
  Serial.print("Current Smoke Level: ");
  Serial.println(smokeLevel);

  // Send the live level to Blynk Virtual Pin V1
  Blynk.virtualWrite(V1, smokeLevel);

  // Check if smoke crosses the danger threshold
  if (smokeLevel > SMOKE_THRESHOLD) {
    Serial.println("⚠️ DANGER! Smoke Detected!");
    
    // Sound local buzzer
    digitalWrite(BUZZER_PIN, HIGH); 
    
    // Trigger your high_smoke event for Push Notifications
    Blynk.logEvent("high_smoke", String("Alert! Smoke level has reached: ") + smokeLevel);
  } else {
    // Turn off local buzzer when levels drop back down to safe ranges
    digitalWrite(BUZZER_PIN, LOW); 
  }
}

void setup() {
  Serial.begin(115200);

  // Pin modes
  pinMode(SMOKE_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Connect to Wi-Fi and Blynk using your provided credentials
  Serial.println("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // Check smoke levels every 1 second (1000ms)
  timer.setInterval(1000L, checkSmokeLevel);
}

void loop() {
  Blynk.run();
  timer.run(); 
}