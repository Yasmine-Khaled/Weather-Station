#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <WiFi.h>
#include <SPI.h>
#include <SD.h>

// Replace with your network credentials
const char* ssid = "iphone";
const char* password = "12121212";

const float THRESHOLD_TEMP = 30.0;
const float THRESHOLD_HUM = 70.0;
const float THRESHOLD_ALT = 150.0;
const float THRESHOLD_PRESS = 1100.0;

#define DHTPIN 4       // Pin connected to the DHT22 sensor
#define DHTTYPE DHT11  // DHT sensor type

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

WiFiServer server(80);
unsigned long lastUpdateTime = 0;
bool abnormalConditions = false;

File dataFile;

// Define the SD card chip select pin
#define SD_CS_PIN 5
const char* FILE_NAME = "/data.txt";

void setup() {
  Serial.begin(115200);
  dht.begin();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1);
  }
  pinMode(26, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  server.begin();
  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

   // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Error initializing SD card.");
    return;
  }
  
  Serial.println("SD card initialized.");
  // Create a new file on the SD card
File file = SD.open(FILE_NAME, FILE_WRITE);
  if (file) {
    file.println("Temperature, Humidity, Altitude, Pressure");
    file.close();
  } else {
    Serial.println("Error opening data file!");
  }
}

void sendAlert() {
  Serial.println("ALERT: Abnormal weather conditions detected!");
  abnormalConditions = true;
  digitalWrite(26, HIGH);
  delay(1000);
  digitalWrite(26, LOW);
}

void saveData(float temperature, float humidity, float altitude, float pressure) {
  // Read the existing data from the file
  String fileContent = "";
  File file = SD.open(FILE_NAME, FILE_READ);
  if (file) {
    while (file.available()) {
      fileContent += char(file.read());
    }
    file.close();
  } else {
    Serial.println("Error opening data file!");
    return;
  }

  // Append the new data to the existing content
  fileContent += String(temperature) + ", ";
  fileContent += String(humidity) + ", ";
  fileContent += String(altitude) + ", ";
  fileContent += String(pressure) + "\n";

  // Open the file in write mode and overwrite its contents with the updated data
  file = SD.open(FILE_NAME, FILE_WRITE);
  if (file) {
    file.print(fileContent);
    file.close();
  } else {
    Serial.println("Error opening data file!");
  }
}

void loop() {
  // Update sensor data every 5 seconds
  if (millis() - lastUpdateTime >= 5000) {
    lastUpdateTime = millis(); // Update the last update time

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    float altitude = bmp.readAltitude();
    float pressure = bmp.readPressure() / 100.0F;

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, Altitude: ");
    Serial.print(altitude);
    Serial.print(" meters, Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");

    // Save data to the SD card
    saveData(temperature, humidity, altitude, pressure);

    if (temperature > THRESHOLD_TEMP || humidity > THRESHOLD_HUM || altitude > THRESHOLD_ALT || pressure > THRESHOLD_PRESS) {
      sendAlert();
    } else {
      abnormalConditions = false; // Reset abnormal conditions flag
    }
  }

  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');

    if (request.indexOf("/data") != -1) {
      // Respond to the /data endpoint
      String jsonResponse = "{\"temperature\":" + String(dht.readTemperature()) +
                            ",\"humidity\":" + String(dht.readHumidity()) +
                            ",\"altitude\":" + String(bmp.readAltitude()) +
                            ",\"pressure\":" + String(bmp.readPressure() / 100.0) + "}";
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();
      client.println(jsonResponse);
    } else {
      // Respond with the HTML page
      String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
      response += "<!DOCTYPE HTML>\r\n";
      response += "<html>\r\n";
      response += "<head>\r\n";
      response += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\r\n";
      response += "<meta charset=\"UTF-8\">\r\n"; // Specify UTF-8 encoding
      response += "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css\">\r\n"; // Font Awesome stylesheet
      response += "<style>\r\n";
      response += "body { font-family: Arial, Helvetica, sans-serif; }\r\n";
      response += ".container { text-align: center; padding: 20px; }\r\n";
      response += ".data { font-size: 24px; }\r\n";
      response += ".alert { color: red; }\r\n"; // Add style for alert messages
      response += ".icon { font-size: 36px; }\r\n"; // Style for icons
      response += "</style>\r\n";
      response += "<script>\r\n";
      response += "setInterval(updateData, 2000);\r\n";
      response += "function updateData() {\r\n";
      response += "  var xhttp = new XMLHttpRequest();\r\n";
      response += "  xhttp.onreadystatechange = function() {\r\n";
      response += "    if (this.readyState == 4 && this.status == 200) {\r\n";
      response += "      var data = JSON.parse(this.responseText);\r\n";
      response += "      document.getElementById('temperature').innerHTML = '<i class=\"fas fa-thermometer-half icon\"></i> Temperature: ' + data.temperature + ' °C';\r\n";
      response += "      document.getElementById('humidity').innerHTML = '<i class=\"fas fa-tint icon\"></i> Humidity: ' + data.humidity + ' %';\r\n";
      response += "      document.getElementById('altitude').innerHTML = '<i class=\"fas fa-mountain icon\"></i> Altitude: ' + data.altitude + ' meters';\r\n";
      response += "      document.getElementById('pressure').innerHTML = '<i class=\"fas fa-tachometer-alt icon\"></i> Pressure: ' + data.pressure + ' hPa';\r\n";
      response += "      document.getElementById('alertMessage').innerText = '";
      response += abnormalConditions ? "Abnormal weather conditions detected!" : "";
      response += "';\r\n";
      response += "    }\r\n";
      response += "  };\r\n";
      response += "  xhttp.open('GET', '/data', true);\r\n";
      response += "  xhttp.send();\r\n";
      response += "}\r\n";
      response += "</script>\r\n";
      response += "</head>\r\n";
      response += "<body>\r\n";
      response += "<div class=\"container\">\r\n";
      response += "<h1>Sensor Data</h1>\r\n";
      response += "<div class=\"data\" id=\"temperature\"></div>\r\n";
      response += "<div class=\"data\" id=\"humidity\"></div>\r\n";
      response += "<div class=\"data\" id=\"altitude\"></div>\r\n";
      response += "<div class=\"data\" id=\"pressure\"></div>\r\n";
      response += "<div class=\"alert\" id=\"alertMessage\"></div>\r\n";
      response += "</div>\r\n";
      response += "</body>\r\n";
      response += "</html>\r\n";

      client.print(response);
    }

    delay(10);
    client.stop();
  }
}