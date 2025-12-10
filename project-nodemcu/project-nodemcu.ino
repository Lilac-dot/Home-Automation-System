#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <ThingSpeak.h>

SoftwareSerial arduino(D6, D7);   // RX, TX

char ssid[] = "Wifi_name";
char pass[] = "PASSWORD";

WiFiClient client;
unsigned long myChannelNumber = ThingSpeak-channel-number;
const char *myWriteAPIKey = "ThingSpeak_Write_API_Key";

float temp = 0;
float hum = 0;
int distanceCM = 0;
int ldrValue = 0;

// Blynk pins
#define VPIN_MANUAL_MODE V0
#define VPIN_LIGHT_SW    V1
#define VPIN_FAN_SW      V2
#define VPIN_DOOR_SW     V3

BlynkTimer timer;

void sendCmd(char c) {
  arduino.write(c);
  Serial.print("Sent to Arduino: ");
  Serial.println(c);
}

void readFromArduino() {
  while (arduino.available()) {
    String data = arduino.readStringUntil('\n');
    Serial.print("Received from Arduino: ");
    Serial.println(data)

    int tIndex = data.indexOf("T:");
    int hIndex = data.indexOf("H:");
    int dIndex = data.indexOf("D:");
    int lIndex = data.indexOf("L:");

    if (tIndex != -1)
      temp = data.substring(tIndex + 2, data.indexOf(' ', tIndex)).toFloat();

    if (hIndex != -1)
      hum = data.substring(hIndex + 2, data.indexOf(' ', hIndex)).toFloat();

    if (dIndex != -1)
      distanceCM = data.substring(dIndex + 2, data.indexOf(' ', dIndex)).toInt();

    if (lIndex != -1)
      ldrValue = data.substring(lIndex + 2).toInt();
  }
}

//send to thingspeak
void sendToThingSpeak() {
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);
  ThingSpeak.setField(3, distanceCM);
  ThingSpeak.setField(4, ldrValue);

  int status = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  Serial.print("ThingSpeak update status: ");
  Serial.println(status);
}

void setup() {
  Serial.begin(9600);
  arduino.begin(9600);

  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  ThingSpeak.begin(client);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, readFromArduino);     
  timer.setInterval(15000L, sendToThingSpeak);   
}

//blynk
BLYNK_WRITE(VPIN_MANUAL_MODE) {
  sendCmd(param.asInt() ? 'M' : 'm');
}

BLYNK_WRITE(VPIN_LIGHT_SW) {
  sendCmd(param.asInt() ? 'L' : 'l');
}

BLYNK_WRITE(VPIN_FAN_SW) {
  sendCmd(param.asInt() ? 'F' : 'f');
}

BLYNK_WRITE(VPIN_DOOR_SW) {
  sendCmd(param.asInt() ? 'D' : 'd');
}

void loop() {
  Blynk.run();
  timer.run();
}
