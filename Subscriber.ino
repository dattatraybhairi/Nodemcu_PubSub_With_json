
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message received in topic: ");
  Serial.print(topic);
  Serial.print("   length is:");
  Serial.println(length);

  Serial.print("Data Received From Broker:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");
  Serial.println();

}
