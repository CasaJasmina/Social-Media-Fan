void setAp() {

  Serial.println(F("Access Point Web Server"));

  // by default the local IP address of will be 192.168.1.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));
  // print the network name (SSID);
  Serial.print(F("Creating access point named: "));
  Serial.println(ssid);
  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid);
   Serial.print("zero");

  if (status != WL_AP_LISTENING) {
    Serial.println(F("Creating access point failed"));
    // don't continue
    while (true);
  }
  Serial.print("one");

  // wait 10 seconds for connection:
  delay(10000);
  // start the web server on port 80
  server.begin();
  Serial.print("two");
  // you're connected now, so print out the status
  printWiFiStatus();
  Serial.print("three");

}

