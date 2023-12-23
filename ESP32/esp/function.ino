void sendRequest() {
  if(millis() - lastRequestTime > requestTime) {
    WiFiClient cloudClient;

    String request = "/update?api_key=" + String(apiKey) + "&field1=" + String(humi) + "&field2=" + String(waterLevel)+ "&field3=" + String(soilMoist)+ "&field4=" + String(temp);
    while(!cloudClient.connect(host, 80)) {
      Serial.println("Thingspeak connection fail");
      delay(1000);
    }
    cloudClient.print(String("GET ") + request + " HTTP/1.1\r\n"
                + "Host: " + host + "\r\n"
                + "Connection: close\r\n\r\n");
    delay(500);
    cloudClient.stop();
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

void pumpWater() {
  if (soilMoistLimit == 0) return;
  if(soilMoist <= soilMoistLimit) {
    digitalWrite(pumpPin, HIGH);
  }else {
    digitalWrite(pumpPin, LOW);
  }
}



