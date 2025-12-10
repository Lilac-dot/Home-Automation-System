# Smart Home Automation System

This repository contains the implementation of a smart home automation system using Arduino UNO and ESP8266 NodeMCU. The system integrates sensors, actuators, wireless communication, and cloud monitoring to automate household functions such as lighting, door control, and temperature-based ventilation. 


## Overview

The system operates in two modes:

* **Automatic Mode:** Sensor readings directly control actuators (LED, DC motor, servo).
* **Manual Mode:** The user overrides automation through the Blynk mobile application.

Selected data (temperature, humidity, distance, light intensity) is uploaded to ThingSpeak for cloud-based monitoring. The project demonstrates a low-cost, scalable approach to smart home automation.


## Objectives

* Automate lights, fans, and door control based on sensor inputs.
* Enable manual override using ESP8266 and the Blynk app.
* Upload sensor data to ThingSpeak for real-time monitoring.
* Develop a modular and cost-efficient home automation prototype.


## Hardware Used

* Arduino UNO
* NodeMCU ESP8266
* Ultrasonic Sensor (HC-SR04)
* DHT11 Temperature & Humidity Sensor
* LDR (Light Dependent Resistor)
* SG90 Servo Motor
* LED, DC Motor


## System Description

* The **LDR** controls lighting based on ambient brightness.
* The **ultrasonic sensor** detects presence and triggers door operation via the servo motor.
* The **DHT11** sensor controls a DC motor acting as a fan based on temperature thresholds.
* The **ESP8266** handles manual control via Blynk and sends collected sensor data to ThingSpeak.
* Arduino and NodeMCU communicate using serial commands to synchronize automation and manual override.


## Code Structure

* **Arduino UNO Code:** Handles sensors, actuators, automatic mode logic, and serial communication with ESP8266.
* **NodeMCU Code:** Connects to Wi-Fi, processes Blynk inputs, receives sensor data, and uploads fields to ThingSpeak.



## Results

The system functioned correctly in both automatic and manual modes. Sensors responded reliably, actuators performed expected actions, and cloud updates through ThingSpeak were successful. The project successfully demonstrates a complete and functional smart home automation prototype.



## Conclusion

This project validates the integration of embedded systems with IoT platforms for practical home automation. It achieves its objectives and provides a foundation for future enhancements such as adding more appliances, security modules, or advanced analytics.


