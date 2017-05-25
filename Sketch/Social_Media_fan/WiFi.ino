

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F("signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
  // print where to go in a browser:
  Serial.print(F("To see this page in action, open a browser to http://"));
  Serial.println(ip);
}

void getWiFi () {
  unsigned long timer;
  timer = millis();
  if (network == "" or password == "") {
    Serial.println(F("Invalid WiFi credentials"));
  }
  Serial.println(F("Attempting to connect to WiFi"));
  WiFi.begin(network, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis()-timer > 10000) {
      Serial.println(F("WiFi credentials are incorrect"));
      network="";
      password="";
      thing_name="";
      needCredentials = true;
      WiFi.end();
      setAp();
      break;
    }
  }
  Serial.println(F("WiFi connection successful"));
  printWiFiStatus();
  needWiFi = false;
  delay(1000);
}


