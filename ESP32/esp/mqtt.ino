void mqttConnect() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("No wifi for MQTT, turning on no wifi mode");
    digitalWrite(pumpPin, LOW);
    noWifiMode();
  }
  // Dont use while because might block the main loop
  if(!client.connected()) {
    Serial.println("Attemping MQTT connection...");
    String clientId = "TienDucESP-";
    clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str())) {
      Serial.println("MQTT connected");

      //***Subscribe all topic you need***
      //client.subscribe("tienduc/123/led");
      client.subscribe((potID + "/pump").c_str());
      client.subscribe((potID + "/moistLimit").c_str());
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
  // if(String(topic) == "tienduc/123/led") {
  //   Serial.print("Changing LED output to ");
  //   if(strMsg == "on"){
  //     Serial.println("on");
  //     digitalWrite(ledPin, HIGH);
  //   }
  //   else if(strMsg == "off"){
  //     Serial.println("off");
  //     digitalWrite(ledPin, LOW);
  //   }
  // }
  if(String(topic) == (potID + "/pump")) {
    Serial.print("Changing PUMP to ");
    if(strMsg == "on"){
      Serial.println("on");
      digitalWrite(pumpPin, HIGH);
    }
    else if(strMsg == "off"){
      Serial.println("off");
      digitalWrite(pumpPin, LOW);
    }
  }
  else if(String(topic) == (potID + "/moistLimit")) {
    Serial.print("Changing Moist Limit to ");
    Serial.println(strMsg);
    soilMoistLimit = strMsg.toInt();
    if(soilMoistLimit == 0) {
      digitalWrite(pumpPin, LOW);
    }
  }
}


