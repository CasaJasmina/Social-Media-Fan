#include "ArduinoLowPower.h"
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoJson.h>

WiFiSSLClient client;
WiFiSSLClient client_two;
char ssid[] = "MKR1000AP"; // created AP name
#include <FlashStorage.h>
typedef struct {
  boolean valid;
  char stored_network[100];
  char stored_password[100];
  char stored_thing_name[100];
} storedCredentials;

FlashStorage(my_flash_store, storedCredentials);
storedCredentials cred;

boolean readingNetwork = false;
boolean readingPassword = false;
boolean readingThing_name = false;
boolean need_HTTP_Request = true;

String password = "";
String network = "";
String thing_name = "";
String oldId = "";

boolean needCredentials = true;
boolean needWiFi = false;

const char* host = "dweet.io";  //this address is given by Github itself
const int httpsPort = 443;

int fanPin = 7;
int keyIndex = 0;                // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);

  my_flash_store.read(&cred);

  if (cred.valid == true) {
    Serial.println("WiFi credentials already known\n");
    needCredentials = false;
    needWiFi = true;
    network = cred.stored_network;
    password = cred.stored_password;
    thing_name = cred.stored_thing_name;
  } else {
    setAp();
  }
  LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, WakeUp, CHANGE);
}
void loop() {

  if (needCredentials) {
    getCredentials();
  }
  if (needWiFi) {
    getWiFi();
  }
  if (need_HTTP_Request && !needCredentials && !needWiFi) {
    Serial.println("starting request");
    HTTP_Request();
    Serial.println("end of request");
  }
  if (!needCredentials && !needWiFi && !need_HTTP_Request) {
    Serial.println("Going to sleeep");
    LowPower.sleep(60000*60);
  }
}

void WakeUp() {
  Serial.println("awake");
  need_HTTP_Request = true;
}

void HTTP_Request() {
  Serial.println("request called");
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("in");
    WiFi.begin(network, password);
  }

  if (client_two.connect(host, httpsPort)) {

    String GetRequest = "GET /get/dweets/for/" + thing_name + " HTTP/1.1";

    client_two.println(GetRequest);
    client_two.println(F("User-Agent: curl/7.37.1"));
    client_two.println(F("Host: dweet.io:443"));
    client_two.println(F("Cache-Control: no-cache"));
    client_two.println(F("Postman-Token: 959d1c9c-e324-c8f7-6279-cbf0411fe41b"));
    client_two.println("");

    Serial.println("request sent");
    String line = "";

    while (client_two.connected()) {
      line = client_two.readStringUntil('\n');
      if (line == "\r") {
        Serial.println(F("headers received"));
        line = client_two.readStringUntil('\r');
        break;
      }
    }
    Serial.println(line);
    DynamicJsonBuffer  jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(line);
    if (!root.success()) {
      Serial.println(F("parseArray() failed"));
    }

    int boost = 0;
    for (int a = 0; a < 5; a++) {
      if (root["with"][a].as<String>() != "") {

        String id = root["with"][a]["created"];
        if (id == oldId) break;
        boost += root["with"][a]["content"]["boost"].as<int>();
      }
      else Serial.println("null");
    }
    Serial.print("boost: ");
    Serial.println(boost);
    oldId = root["with"][0]["created"].as<String>();
    if (boost != 0) boostFan(boost);
    need_HTTP_Request = false;
    client_two.stop();
  }
}

void boostFan(int boostTime) {
  digitalWrite(fanPin, HIGH);
  delay(boostTime * 1000);
  digitalWrite(fanPin, LOW);
}



