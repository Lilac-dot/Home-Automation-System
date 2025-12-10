#include <DHT.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Servo doorServo;
SoftwareSerial esp(10, 11); // RX, TX

// Pins
int pirPin = 2;
int ledPin = 7;
int trigPin = 8;
int echoPin = 9;
int fanPin = 6;
int ldrPin = A0;

// Manual flags
bool manualLight = false;
bool manualFan = false;
bool manualDoor = false;
bool manualMode = false;

void setup() {
  Serial.begin(9600);
  esp.begin(9600);

  dht.begin();
  doorServo.attach(5);

  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(ldrPin, INPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(fanPin, LOW);
  doorServo.write(0);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;   // No echo
  return duration * 0.034 / 2;
}

void loop() {

  if (esp.available()) {
    char cmd = esp.read();
    Serial.print("Received CMD: ");
    Serial.println(cmd);

    if (cmd == 'M') manualMode = true;
    else if (cmd == 'm') manualMode = false;

    else if (cmd == 'L') manualLight = true;
    else if (cmd == 'l') manualLight = false;

    else if (cmd == 'F') manualFan = true;
    else if (cmd == 'f') manualFan = false;

    else if (cmd == 'D') manualDoor = true;
    else if (cmd == 'd') manualDoor = false;
  }

  //sensor readings
  int motion = digitalRead(pirPin);
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  int distance = getDistance();
  int ldrValue = analogRead(ldrPin);
  bool object = (distance < 10);
  bool isDark = (ldrValue < 600);

  //LIGHT
  if (manualMode) {
    digitalWrite(ledPin, manualLight ? HIGH : LOW);
  } else {
    if (motion && object && isDark)
      digitalWrite(ledPin, HIGH);
    else
      digitalWrite(ledPin, LOW);
  }

//FAN
  if (manualMode) {
    digitalWrite(fanPin, manualFan ? HIGH : LOW);
  } else {
    if (temp > 25 && motion && object)
      digitalWrite(fanPin, HIGH);
    else
      digitalWrite(fanPin, LOW);
  }

//DOOR
  if (manualMode) {
    doorServo.write(manualDoor ? 90 : 0);
  } else {
    if (object) doorServo.write(90);
    else doorServo.write(0);
  }

//data to send to nodemcu for thingspeak
  esp.print("T:");
  esp.print(temp);

  esp.print(" H:");
  esp.print(hum);

  esp.print(" D:");
  esp.print(distance);

  esp.print(" L:");
  esp.println(ldrValue);

  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Hum: "); 
  Serial.println(hum);
  Serial.print("Distance: "); 
  Serial.println(distance);
  Serial.print("LDR: "); 
  Serial.println(ldrValue);

  delay(800);
}
