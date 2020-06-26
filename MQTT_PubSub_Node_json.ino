#include <UIPEthernet.h>
#include "PubSubClient.h"
#define selectpin 2
#define CLIENT_ID       "UnoMQTT"
#define INTERVAL       3000 // 3 sec delay between publishing
#define no_tags  20
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
uint8_t Array[1000] = {0x5A, 0xC2, 0x15, 0xD3, 0x00, 0x01, 0x00, 0x98, 0x18, 0x59, 0x53, 0x51, 0x4A, 0x08, 0x00, 0x00, 0x62, 0x1A, 0x4C, 0x4C, 0x47, 0x47, 0x08, 0x01};
uint8_t cpy[] = {0xD3, 0x00, 0x01, 0x00, 0x98, 0x18, 0x59, 0x53, 0x51};
long previousMillis;

EthernetClient ethClient;
PubSubClient mqttClient;


void setup() {
  // setup serial communication
  Serial.begin(115200);
  Ethernet.init(selectpin);
  // setup ethernet communication using DHCP
  if (Ethernet.begin(mac) == 0) {
    //Serial.println(F("Ethernet configuration using DHCP failed"));
    //for (;;);
    yield();
  }
  Serial.println(Ethernet.localIP());
  // setup mqtt client
  mqttClient.setClient(ethClient);
  mqttClient.setServer("192.168.1.40", 1883);
  mqttClient.setCallback(callback);
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");

    if (mqttClient.connect("UnoMQTT"))
    {

      Serial.println("connected to MQTT broker");

    }
    else
    {

      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(500);

    }
  }


  Serial.println("ESP8266 AS SUBSCRIBER");
  Serial.println("Subscribing topic test:");
  mqttClient.subscribe("test");//topic name="abc"
  Serial.println(F("MQTT client configured"));

  // setup DHT sensor
  previousMillis = millis();

  for (int i = 0; i < no_tags; i++ ) {
    memcpy(&Array[22 + (i * 9)], cpy, 9);
  }
}

void loop() {
  // check interval

  //  if (millis() - previousMillis > INTERVAL) {
  //    sendData();
  //    previousMillis = millis();
  //    yield();
  //  }




  mqttClient.loop();
}
void sendData() {



  String payload1 = "{"; payload1 += "\"id\":"; payload1 += "10"; payload1 += ",";  payload1 += "\"Zid\":"; payload1 += "\"";
  payload1 += String(Array[0], HEX); payload1 += "-"; payload1 += String(Array[1], HEX); payload1 += "-"; payload1 += String(Array[2], HEX); payload1 += "-";
  payload1 +=  String(Array[3], HEX); payload1 += "-";  payload1 += String(Array[4], HEX);
  payload1 += "-"; payload1 +=  String(Array[5], HEX); payload1 += "\""; payload1 += ","; payload1 += "\"zbt\":"; payload1 += "\"FEE\""; payload1 += ",";
  payload1 += "\"ist\":"; payload1 += "1"; payload1 += ","; payload1 += "\"Temperature\":";  payload1 += "1"; payload1 += ","; payload1 += "\"Humidity\":";
  payload1 += "1"; payload1 += ","; payload1 += "\"X\":";  payload1 += "100"; payload1 += ","; payload1 += "\"Y\":";
  payload1 += "100"; payload1 += ","; payload1 += "\"tags\":";  payload1 +=  "[";

  for (int i = 0; i < no_tags ; i++) {
    payload1 +=  "{"; payload1 += "\"tid\":";
    payload1 += "\"";
    payload1 += String(Array[0], HEX); payload1 += "-";
    payload1 += String(Array[1], HEX); payload1 += "-";
    payload1 += String(Array[2], HEX); payload1 += "-";
    payload1 += String(Array[3], HEX); payload1 += "-";
    payload1 += String(Array[9 * (i + 1) - 3], HEX); payload1 += "-";
    payload1 += String(Array[9 * (i + 1) - 2], HEX); payload1 += "\""; payload1 += ",";

    payload1 += "\"tbt\":"; payload1 += Array[9 * (i + 1) - 1]; payload1 += ",";
    payload1 += "\"V1\":"; payload1 += Array[9 * (i + 1) ]; payload1 += ",";
    payload1 += "\"V2\":"; payload1 += Array[9 * (i + 1) + 1]; payload1 += ",";
    payload1 += "\"V3\":"; payload1 += Array[9 * (i + 1) + 2]; payload1 += ",";
    payload1 += "\"V4\":"; payload1 += Array[9 * (i + 1) + 3]; payload1 += ",";
    payload1 += "\"toc\":"; payload1 += Array[9 * (i + 1) + 4]; payload1 += ",";
    payload1 += "\"P\":"; payload1 += Array[9 * (i + 1) + 5];
    payload1 += "}";
    if (i < (no_tags - 1)) {
      payload1 += ",";
    }
  }
  payload1 += "]";
  payload1 += "}";
  // Serial.println(payload1);


  uint16_t  str_len = payload1.length() + 1;
  Serial.println(str_len);
  char* attributes = (char*)malloc(10240);
  payload1.toCharArray( attributes, str_len );
  Serial.println(attributes);

  if (mqttClient.connect(CLIENT_ID)) {
    mqttClient.publish("api", attributes);
    Serial.println(F("PUBLISHED"));
  }
  free(attributes);
  attributes = NULL;
}
