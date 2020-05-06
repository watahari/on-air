/*
    Name:       ON-AIR light controller
    Created:    2020/05/06
    Author:     watahari
*/

#include <M5Stack.h>
#include <WiFi.h>

const char* ssid        = "XXXX"; // your network SSID (name of wifi network)
const char* password    = "XXXX"; // your network password
const char* webhook_key = "XXXX"; // your IFTTT webhook key
const bool  debug       = false;  // display debug mode

// The setup() function runs once each time the micro-controller starts
void setup() {
  // init lcd, serial, but don't init sd card
  M5.begin();
  
  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();

  // Setup Wi-Fi
  wifi_setup();

  display("default");
}

// Add the main program code into the continuous loop() function
void loop() {
  // update button state
  M5.update();

  // M5.BtnA.wasPressed() conflicts with WiFi?
  // https://github.com/m5stack/M5Stack/issues/52
  // https://github.com/espressif/arduino-esp32/issues/1334
  
  if (M5.BtnB.wasReleased()) {
    M5.Lcd.setTextColor(RED);
    post_ifttt("on_air");
    if (debug) {
      M5.Lcd.println(">> ON-air <<");
    } else {
      display("on");
    }
  } else if (M5.BtnC.wasReleased()) {
    M5.Lcd.setTextColor(BLUE);
    post_ifttt("off_air");
    if (debug) {
      M5.Lcd.println(">> Off-air <<"); 
    } else {
      display("off");
    }
  } else if (M5.BtnA.wasReleasefor(700)) {
    display("default");
  }
}


void post_ifttt(String event) {
  const char* host = "maker.ifttt.com";
  String url1 = "/trigger/";
  String url2 = "/with/key/";
  String url = url1 + event + url2 + webhook_key;
  if (debug) {
    M5.Lcd.print("Requesting URL:");
    M5.Lcd.println(url);
  }

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
  }
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
      }
  }
}

void display(String mode) {
  if (mode == "default") {
    M5.Lcd.clear(BLACK);
  }
  if (!debug) {
    M5.Lcd.clear(BLACK);
    if (mode == "default") {
      M5.Lcd.drawRoundRect(112,120,98,118,4,RED);
      M5.Lcd.setTextColor(RED);
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(120, 150);
      M5.Lcd.println("ON");
      M5.Lcd.drawRoundRect(213,120,98,118,4,BLUE);
      M5.Lcd.setTextColor(BLUE);
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(220, 150);
      M5.Lcd.println("OFF");
    } else if (mode == "on") {
      M5.Lcd.fillRoundRect(112,120,98,118,4,RED);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(120, 150);
      M5.Lcd.println("ON");
      M5.Lcd.drawRoundRect(213,120,98,118,4,BLUE);
      M5.Lcd.setTextColor(BLUE);
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(220, 150);
      M5.Lcd.println("OFF");
    } else if (mode == "off") {
      M5.Lcd.drawRoundRect(112,120,98,118,4,RED);
      M5.Lcd.setTextColor(RED);
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(120, 150);
      M5.Lcd.println("ON");
      M5.Lcd.fillRoundRect(213,120,98,118,4,BLUE);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(220, 150);
      M5.Lcd.println("OFF");
    }
  }
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.println("ON-AIR controller");
  M5.Lcd.setCursor(0, 35);
  M5.Lcd.println("Press button B to ON-air");
  M5.Lcd.println("Press button C to OFF-air");
  if (!debug) {
    M5.Lcd.setCursor(20,220);
    M5.Lcd.println("reset");
  }
}

void wifi_setup(){
  M5.Lcd.setTextSize(2);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.print("\r\nWiFi connected\r\nIP address: ");
  M5.Lcd.println(WiFi.localIP());

  delay(5000);
}
