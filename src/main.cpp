#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include "./ArduinoSecrets.h"
#include <dht11.h>

/////// WiFi Settings ///////
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;

// server address
char serverAddress[] = SERVER_ADDRESS;
int port = 9090;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

// temperature in Celsius
float tempC; 

// temperature in Fahrenheit
float tempF; 

int DHT11Temp = 0;
int DHT11Hum = 0;
const int DHT11pin = 23;

dht11 DHT;

// DHT11 Sensor POST data
// void dht11_post() {

// }

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Connecting to: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  DHT.read(DHT11pin);
  DHT11Temp = DHT.temperature;
  DHT11Hum = DHT.humidity;

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.println("Current Env Temperature: " + DHT11Temp);
  Serial.println("Current Humidity: " + DHT11Hum);
  Serial.println("making POST request");

  // Env Temperature POST data
  String envTempData = "{temperature:";
  String envTempData01 = "}";
  envTempData = envTempData + DHT11Temp + envTempData01;

  // Humidity POST data
  // String humidityData = "{temperature:";
  // String humidityData01 = "}";
  // humidityData = humidityData + DHT11Hum + humidityData01;

  Serial.print("Current JSON BODY: " + envTempData);
  client.beginRequest();
  client.post("/api/v1/UzNLA5VIxF6TpnEv15yj/telemetry");
  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/json");
  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, envTempData.length());
  client.sendHeader("X-CUSTOM-HEADER", "custom_value");
  client.endRequest();
  client.write((const byte*)envTempData.c_str(), envTempData.length());
  // note: the above line can also be achieved with the simpler line below:
  //client.print(envTempData);

 
  Serial.print("POST Status code: ");
  Serial.println(statusCode);
  Serial.print("POST Response: ");
  Serial.println(response);

  Serial.println("Wait five seconds");
  delay(5000);
}