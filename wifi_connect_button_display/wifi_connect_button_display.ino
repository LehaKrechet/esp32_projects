#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <secret.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define LED_PIN 4
#define BUTTON_PIN 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWD";

WiFiServer server(80);  // Порт 80

void wifi_on_off(bool bState){
  bool flag = true;
  if (bState == true and flag == true){
    WiFi.begin(ssid, password);
    display.clearDisplay();
    display.setCursor(0,0);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");

      delay(1000);
      display.print(".");

      display.display();
    }

    
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("\nWiFi connection IP: ");
    display.println(WiFi.localIP());
    display.display();

    digitalWrite(2, HIGH);
    server.begin();
    flag = false;
  }else{
    
    WiFi.disconnect();

    display.clearDisplay();
    display.setCursor(0,0);
    display.println("WiFi OFF");
    display.display();

    digitalWrite(2, LOW);
    server.stop();
    delay(300);
  }
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  int count_msg = 0;
  int but = !digitalRead(BUTTON_PIN);
  static bool bState = false;
  static bool flag = true;
  if (but == 1){
    bState = !bState;
  }
  if (flag and bState == 1){
    wifi_on_off(bState);
    flag = false;
  }else if (flag or bState == 0){
    wifi_on_off(bState); 
    flag = true;
  }
  
  

  WiFiClient client = server.available();
  
  if (client) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Client connection");
    client.println("Connected");
    display.display();
    delay(500);
    
    while (client.connected()) {
      but = !digitalRead(BUTTON_PIN);
      if (but == 1){
        bState = !bState;
      }
      if (client.available()) {
        String data = client.readStringUntil('\n');
        data.trim();

        display.print("Received: ");
        display.println(data);
        count_msg += 1;
        display.display();

        if (data == "1"){
            digitalWrite(LED_PIN, HIGH);
        }else if (data == "0"){
            digitalWrite(LED_PIN, LOW);
        }
        
        // Ответ клиенту
        client.print("ESP32 получил: ");
        client.println(data);
      }
      if (count_msg == 8) {
        count_msg = 0;
        display.clearDisplay();
        display.setCursor(0, 0);
      }
      if (bState == 0){
        delay(500);
        client.println("Disconnect");
        delay(500);
        break;
      }
    }
    client.println("Disconnect");
    client.stop();
    display.println("Client disonection");
    display.display();
    delay(1000);
  }
}
