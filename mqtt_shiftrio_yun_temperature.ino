// This example uses an Arduino Yun or a Yun-Shield
// and the MQTTClient to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <Bridge.h>
#include <BridgeClient.h>
#include <MQTT.h>
#include <math.h>

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0

BridgeClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  //Serial.print("connecting...");
  while (!client.connect("arduino-yun-vizinviz", "ecf929fe", "f65bde19d8e46d67")) {
    // Serial.print(".");
  }

  //Serial.println("\nconnected!!!!!");

  //client.subscribe("/velo");
  //client.subscribe("/hello");
  //client.subscribe("/temperature");

  // client.unsubscribe("/hello");
}

/*void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  }*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Bridge.begin();
  //Serial.begin(115200);
  // Serial.println("\nhi");
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("broker.shiftr.io", net);
  //client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 2000) {
    lastMillis = millis();

    int a = analogRead(pinTempSensor);
    float R = 1023.0 / a - 1.0;
    R = R0 * R;

    float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet

    String topic = "/temperature";
    String payload = "";
    payload += temperature;
    digitalWrite(LED_BUILTIN, HIGH);
    client.publish(topic, payload);
    //Serial.println("published: " + topic + " - " + payload);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

  }


}
