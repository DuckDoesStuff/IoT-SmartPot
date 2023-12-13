void sendRequest() {
  if(millis() - lastRequestTime > requestTime) {
    WiFiClient cloudClient;
    int waterLevel = readWater();
    int soilMoist = readMoist();

    String request = "/update?api_key=" + String(apiKey) + "&field1=" + String(waterLevel) + "&field2=" + String(soilMoist);
    while(!cloudClient.connect(host, 80)) {
      Serial.println("connection fail");
      delay(1000);
    }
    cloudClient.print(String("GET ") + request + " HTTP/1.1\r\n"
                + "Host: " + host + "\r\n"
                + "Connection: close\r\n\r\n");
    delay(500);

    while(cloudClient.available()) {
      String line = cloudClient.readStringUntil('\r');
      Serial.print(line);
    }
    lastRequestTime = millis();
    Serial.println("Sent to thingspeak");
  }
}

int readWater() {
  int sensorValue = analogRead(waterLevelPin);
  int outputValue = map(sensorValue, 0, 2000, 0, 5);
  return outputValue;
}

int readMoist() {
  int sensorValue = analogRead(soilMoistPin);
  int outputValue = map(sensorValue, 0, 4095, 100, 0);
  return outputValue;
}