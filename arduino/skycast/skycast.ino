#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

// only use 2.4ghz
#define WIFI_SSID "your wifi ssid"
#define WIFI_PASSWORD "wifi pass"

Adafruit_BME280 bme;
WiFiClient wifiClient;
FirebaseData firebaseData;

void connectToWiFi()
{

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi");
}

void setup()
{

  Serial.begin(115200);

  connectToWiFi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }
}

void loop()
{

  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0; // Convert pressure to hPa

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read data from BME280 sensor");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  if (Firebase.ready())
  {
    Firebase.setFloat(firebaseData, "/temp", temperature);
    Firebase.setFloat(firebaseData, "/humid", humidity);
    Firebase.setFloat(firebaseData, "/pressure", pressure);
  }
  else
  {
    Serial.println("Firebase not ready");
  }

  delay(5000);
}
