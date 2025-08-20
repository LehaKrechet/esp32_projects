#include <WiFi.h>
#include <secret.h>
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWD";

WiFiServer server(80);  // Порт 80

void setup() {
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  
  Serial.print("\nWiFi подключён. IP: ");
  digitalWrite(2, HIGH);
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Сервер запущен");
  
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Клиент подключен");
    client.println("Connected");
    
    while (client.connected()) {
      if (client.available()) {
        String data = client.readStringUntil('\n');
        data.trim();
        
        Serial.print("Получено: ");
        Serial.println(data);

        if (data == "1"){
            digitalWrite(4, HIGH);
        }else if (data == "0"){
            digitalWrite(4, LOW);
        }
        
        // Ответ клиенту
        client.print("ESP32 получил: ");
        client.println(data);
      }
    }
    
    client.stop();
    Serial.println("Клиент отключен");
  }
}
