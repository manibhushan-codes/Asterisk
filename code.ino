#include <WiFi.h>
#include <DHT.h>

// DHT Sensor setup
#define DHTPIN 4          // GPIO pin where DHT22 is connected
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Actuator pins
#define FAN_PIN 16        // GPIO for fan
#define HUMIDIFIER_PIN 17 // GPIO for humidifier

// Thresholds
const float TEMP_MIN = 23.0;
const float TEMP_MAX = 28.0;
const float HUMIDITY_MIN = 70.0;
const float HUMIDITY_MAX = 85.0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(FAN_PIN, OUTPUT);
  pinMode(HUMIDIFIER_PIN, OUTPUT);

  digitalWrite(FAN_PIN, LOW);
  digitalWrite(HUMIDIFIER_PIN, LOW);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.printf("Temp: %.1f°C | Humidity: %.1f%%\n", temperature, humidity);

  // Temperature control
  if (temperature > TEMP_MAX) {
    digitalWrite(FAN_PIN, HIGH);  // Turn on fan
    Serial.println("Fan ON: Cooling down...");
  } else if (temperature < TEMP_MIN) {
    digitalWrite(FAN_PIN, LOW);   // Turn off fan
    Serial.println("Fan OFF: Temperature is ideal.");
  }

  // Humidity control
  if (humidity < HUMIDITY_MIN) {
    digitalWrite(HUMIDIFIER_PIN, HIGH);  // Turn on humidifier
    Serial.println("Humidifier ON: Increasing humidity...");
  } else if (humidity > HUMIDITY_MAX) {
    digitalWrite(HUMIDIFIER_PIN, LOW);   // Turn off humidifier
    Serial.println("Humidifier OFF: Humidity is ideal.");
  }

  delay(5000); // Wait before next reading
}
