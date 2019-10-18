#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <WiFi.h>
#include <WiFiMulti.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (997)

Adafruit_BME680 bme; // I2C
//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

const String uri = "/";
const char *host = "94.237.118.75";
const uint16_t port = 7778;
const int32_t timeout = 500;

WiFiMulti WiFiMulti;

void setup()
{
    Serial.begin(115200);
  while (!Serial);
  Serial.println(F("BME680 async test"));

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

 WiFiMulti.addAP("TP-Link_DDC9", "patlican123");
 Serial.println();
 Serial.println();
 Serial.print("WiFi Connecting...");
 
 while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

  Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

       delay(500);
}

void loop()
{
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }
  Serial.print(F("Reading started at "));
  Serial.print(millis());
  Serial.print(F(" and will finish at "));
  Serial.println(endTime);

  Serial.println(F("You can do other work during BME680 measurement."));
  delay(50); // This represents parallel work.
  // There's no need to delay() until millis() >= endTime: bme.endReading()
  // takes care of that. It's okay for parallel work to take longer than
  // BME680's measurement time.

  // Obtain measurement results from BME680. Note that this operation isn't
  // instantaneous even if milli() >= endTime due to I2C/SPI latency.
  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  Serial.print(F("Reading completed at "));
  Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(bme.temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Pressure = "));
  Serial.print(bme.pressure / 100.0);
  Serial.println(F(" hPa"));

  Serial.print(F("Humidity = "));
  Serial.print(bme.humidity);
  Serial.println(F(" %"));

  Serial.print(F("Gas = "));
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(F(" KOhms"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(F(" m"));

  Serial.println();

   Serial.print("Connecting to ");
   Serial.println(host);

   WiFiClient client;
   
    if (!client.connect(host, port, timeout)) {
        Serial.println("Connection failed.");
        Serial.println("Waiting 5 seconds before retrying...");
        delay(5000);
        return;
    }

    
  String body = 
  "temperature=" + String(bme.temperature) +
  "&humidity=" + String(bme.humidity) +
  "&pressure=" + String(bme.pressure/100) +
  "&gas=" + String(bme.gas_resistance/1000) +
  "&altitude=" + String(bme.readAltitude(SEALEVELPRESSURE_HPA)); 
  
  String postRequest = 
  "POST " + uri + " HTTP/1.1\r\n" + 
  "Host: " + host + "\r\n" + 
  "Content-Type: application/x-www-form-urlencoded\r\n" +
  "Content-Length: " + body.length() + "\r\n" +
  "\r\n" + body;

  Serial.println(postRequest);
  client.print(postRequest);
  client.stop();
  Serial.println();
  Serial.println("***DONE***");
  Serial.println();
  delay(6000);
}
