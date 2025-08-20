#include <WiFi.h>

const char* ssid = "Klan_Krecheta3";
const char* password = "werthvfy12";

WiFiServer server(80);  // Порт 80

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  
  Serial.println("\nWiFi подключён. IP: " + WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    String data = client.readStringUntil('\r');
    Serial.print("Получено: ");
    Serial.println(data);
    client.stop();
  }
}