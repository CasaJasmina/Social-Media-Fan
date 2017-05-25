
void getCredentials() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '?') readingNetwork = true;
        if (readingNetwork) {
          if (c == '!') {
            readingPassword = true;
            readingNetwork = false;
          }
          else if (c != '?') {
            network += c;
          }
        }
        if (readingPassword) {
          if (c == '*') {
            readingThing_name = true;
            readingPassword = false;
          }
          else if (c != '!') {
            password += c;
          }
        }
        if (readingThing_name) {
          if (c == ',') {
            readingThing_name = false;
            Serial.println();
            Serial.print(F("Network Name: "));
            Serial.println(network);
            Serial.print(F("Password: "));
            Serial.println(password);
            Serial.print(F("Thing name: "));
            Serial.println(thing_name);
            Serial.println();
            client.stop();
            WiFi.end();

            network.toCharArray(cred.stored_network, 100);
            password.toCharArray(cred.stored_password, 100);
            thing_name.toCharArray(cred.stored_thing_name, 100);
            cred.valid = true;
            my_flash_store.write(cred);

            needCredentials = false;
            needWiFi = true;
          }
          else if (c != '*') {
            thing_name += c;
          }
        }

        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println(F("HTTP/1.1 200 OK"));
            client.println(F("Content-type:text/html"));
            client.println();
            client.println(F("<html>"));
            client.println(F("<head>"));
            client.println(F("<style type=\"text/css\"> body {font-family: sans-serif; margin:50px; padding:20px; line-height: 250% } </style>"));
            client.println(F("<title>Arduino Setup</title>"));
            client.println(F("</head>"));
            client.println(F("<body>"));

            client.println(F("<h2>WIFI CREDENTIALS</h2>"));
            client.print(F("NETWORK NAME: "));
            client.print(F("<input id=\"network\"/><br>"));
            client.print(F("PASSWORD: "));
            client.print(F("<input id=\"password\"/><br>"));
            client.println(F("<h2>DWEET.IO CREDENTIALS</h2>"));
            client.print(F("YOUR THING NAME: "));
            client.print(F("<input id=\"thing_name\"/><br>"));
            client.print(F("<br>"));
            client.print(F("<button type=\"button\" onclick=\"SendText()\">Enter</button>"));
            client.println(F("</body>"));

            client.println(F("<script>"));
            client.println(F("var network = document.querySelector('#network');"));
            client.println(F("var password = document.querySelector('#password');"));
            client.println(F("var thing_name = document.querySelector('#thing_name');"));

            client.println(F("function SendText() {"));
            client.println(F("nocache=\"&nocache=\" + Math.random() * 1000000;"));
            client.println(F("var request =new XMLHttpRequest();"));
            client.println(F("netText = \"&txt=\" + \"?\" + network.value + \"!\" + password.value + \"*\" + thing_name.value  + \",&end=end\";"));
            client.println(F("request.open(\"GET\", \"ajax_inputs\" + netText + nocache, true);"));
            client.println(F("request.send(null)"));
            client.println(F("network.value=''"));
            client.println(F("password.value=''"));
            client.println(F("thing_name.value=''}"));
            client.println(F("</script>"));

            client.println(F("</html>"));
            client.println();
            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println(F("client disconnected"));
    Serial.println();


  }
}

