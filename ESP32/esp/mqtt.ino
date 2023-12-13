void mqttConnect() {
  while(!client.connected()) {
    Serial.println("Attemping MQTT connection...");
    String clientId = "TienDucESP-";
    clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str())) {
      Serial.println("connected");

      //***Subscribe all topic you need***
      client.subscribe("tienduc/123/led");
      client.subscribe("tienduc/123/pump");
    }
    else {
      Serial.println("try again in 5 seconds");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

//MQTT Receiver
void callback(char* topic, byte* message, unsigned int length) {
  Serial.println(topic);
  String strMsg;
  for(int i=0; i<length; i++) {
    strMsg += (char)message[i];
  }
  Serial.println(strMsg);

  //***Code here to process the received package***
  if (String(topic) == "tienduc/123/led") {
    Serial.print("Changing output to ");
    if(strMsg == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(strMsg == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
  else if (String(topic) == "tienduc/123/pump") {
    Serial.print("Changing output to ");
    if(strMsg == "on"){
      Serial.println("on");
      digitalWrite(pumpPin, HIGH);
    }
    else if(strMsg == "off"){
      Serial.println("off");
      digitalWrite(pumpPin, LOW);
    }
  }
}
