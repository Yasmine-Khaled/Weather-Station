# Weather Station
Weather Station IoT Project with BMP180, DHT11, SD Card Module, ESP32, LED Alert, and Web Interface

## Overview :
The Weather Station IoT project integrates various sensors and modules to create a comprehensive system that monitors and records environmental conditions. The project employs BMP180 and DHT11 sensors to measure temperature, humidity, and atmospheric pressure. Data is stored on an SD card module for historical analysis, and an ESP32 microcontroller is used to process sensor readings, trigger alerts with LEDs, and provide a web interface for real-time monitoring.

## Components :

1. BMP180 Sensor:

* Measures atmospheric pressure and temperature, providing data for weather forecasting.
2. DHT11 Sensor:

* Measures temperature and humidity, enhancing the weather station's capabilities.
3. SD Card Module:

* Enables the storage of sensor readings on an SD card, allowing for historical data analysis.
4. ESP32 Microcontroller:

* Serves as the central processing unit, collecting data from sensors, managing alerts, and facilitating communication with the web interface.
5. LEDs for Alerts:

* Visual indicators (LEDs) are used to signal certain conditions, such as exceeding predefined temperature or humidity thresholds.
6. Web Interface:

* A user-friendly website hosted on the ESP32 allows users to monitor real-time sensor readings from any device with a web browser.
## Functionality :

1. Sensor Data Acquisition:

* The ESP32 collects temperature, humidity, and atmospheric pressure data from the BMP180 and DHT11 sensors.
2. Data Logging to SD Card:

* The acquired data is logged onto the SD card module, creating a record for future analysis.
3. Alert System:

* LEDs are employed to provide visual alerts when specific conditions are met, such as high temperature or humidity levels. The ESP32 triggers these alerts based on predefined thresholds.
4. Real-time Web Monitoring:

* The ESP32 hosts a web server that displays real-time sensor readings on a user-friendly interface. Users can access this interface through a web browser, allowing remote monitoring of environmental conditions.
5. Historical Data Analysis:
* Users can retrieve historical data from the SD card for in-depth analysis of weather patterns and trends.
  
## Connection
![ESP32-Wi-Fi-Weather-Station-using-DHT11-and-BMP180-Sensor-Circuit-Diagram](https://github.com/Yasmine-Khaled/Weather-Station/assets/89998528/2bfa53f0-9a4b-40ea-92d2-d717a5648c7b)
## Web Interface
![Screenshot 2023-12-14 074300](https://github.com/Yasmine-Khaled/Weather-Station/assets/89998528/3790131c-de26-4826-9053-eaa7de46a9ce)

## Conclusion :
The Weather Station IoT project with BMP180, DHT11, SD Card Module, ESP32, LED Alert, and Web Interface provides a comprehensive solution for monitoring environmental conditions, making it suitable for various applications where real-time and historical data analysis is crucial.Users can retrieve historical data from the SD card for in-depth analysis of weather patterns and trends.                               
Thank You!
