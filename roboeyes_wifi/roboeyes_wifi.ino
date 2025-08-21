#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <secret.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <FluxGarage_RoboEyes.h>
roboEyes roboEyes; // create RoboEyes instance

#define BUTTON_PIN 5
#define LED_PIN 4

unsigned long timer;
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;
WiFiServer server(80);  // Порт 80

String readLineWithTimeout(WiFiClient &client, unsigned long timeoutMs = 5000) {
  String response = "";
  unsigned long startTime = millis();
  char currentChar;
  
  while (millis() - startTime < timeoutMs) {
    if (client.available() > 0) {
      currentChar = client.read();
      
      if (currentChar == '\n') {
        // Найден конец строки - возвращаем результат
        return response;
      } else if (currentChar != '\r') {
        // Игнорируем \r и добавляем полезные символы
        response += currentChar;
      }
      
      // Сброс таймера при получении каждого символа
      startTime = millis();
    }
    
    // Небольшая пауза для снижения нагрузки на процессор
    delay(1);
  }
  
  // Если вышли по таймауту
  Serial.println("Timeout waiting for data");
  return response; // Вернем то, что успели собрать (может быть пустым)
}

void wifi_on_off(bool bState){
  bool flag = true;
  if (bState == true and flag == true){
    WiFi.begin(ssid, password);
    display.clearDisplay();
    display.setCursor(0,0);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
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

void eyes(String data){
  
  static bool flag = true;
  static bool event_right_blink = 0;
  static bool event_left_blink = 0;
  static bool event_laugh = 0;
  static bool event = 0;
  if (flag){
    roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);
    flag = false;
  }
  roboEyes.update();
  if ((millis() > timer + 500) && (event == 0) && (data == "right_blink")){
    roboEyes.blink(0, 1);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "left_blink")){
    roboEyes.blink(1, 0);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "laugh")){
    roboEyes.anim_laugh();
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "confused")){
    roboEyes.anim_confused();
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "blink")){
    roboEyes.blink();
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "open")){
    roboEyes.open();
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "close")){
    roboEyes.close();
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "left_open")){
    roboEyes.open(1, 0);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "left_close")){
    roboEyes.close(1, 0);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "right_open")){
    roboEyes.open(0, 1);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "right_close")){
    roboEyes.close(0, 1);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "mode_default")){
    roboEyes.setMood(DEFAULT);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "mode_tired")){
    roboEyes.setMood(TIRED);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "mode_angry")){
    roboEyes.setMood(ANGRY);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "mode_happy")){
    roboEyes.setMood(HAPPY);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "curiosity_on")){
    roboEyes.setCuriosity(ON);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "sweat_on")){
    roboEyes.setSweat(ON);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "curiosity_off")){
    roboEyes.setCuriosity(OFF);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "sweat_off")){
    roboEyes.setSweat(OFF);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "autoblinker_on")){
    roboEyes.setAutoblinker(ON, 2, 2);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "idlemode_on")){
    roboEyes.setIdleMode(ON, 2, 2);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "autoblinker_off")){
    roboEyes.setAutoblinker(OFF, 2, 2);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "idlemode_off")){
    roboEyes.setIdleMode(OFF, 2, 2);
    event = 1;
  }
  if ((millis() > timer + 500) && (event == 0) && (data == "default_pos")){
    roboEyes.setPosition(DEFAULT);
    event = 1;
  }
  if (millis() > timer + 1000 && event == 1){
    // roboEyes.open(1, 1);
    timer = millis();
    event = 0;
    // event_right_blink = 0;
    // event_left_blink = 0;
    // event_laugh = 0;

  }
  
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);


  // Startup OLED Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);



  // Startup robo eyes
  // roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100); // screen-width, screen-height, max framerate
  // roboEyes.open();

  // Define some automated eyes behaviour
  // roboEyes.setAutoblinker(ON, 3, 2); // Start auto blinker animation cycle -> bool active, int interval, int variation -> turn on/off, set interval between each blink in full seconds, set range for random interval variation in full seconds
  // roboEyes.setIdleMode(ON, 1, 1); // Start idle animation cycle (eyes looking in random directions) -> turn on/off, set interval between each eye repositioning in full seconds, set range for random time interval variation in full seconds
  
  // Define eye shapes, all values in pixels
  //roboEyes.setWidth(36, 36); // byte leftEye, byte rightEye
  //roboEyes.setHeight(36, 36); // byte leftEye, byte rightEye
  //roboEyes.setBorderradius(8, 8); // byte leftEye, byte rightEye
  //roboEyes.setSpacebetween(10); // int space -> can also be negative

  // Cyclops mode
  //roboEyes.setCyclops(ON); // bool on/off -> if turned on, robot has only on eye

  // Define mood, curiosity and position
  //roboEyes.setMood(DEFAULT); // mood expressions, can be TIRED, ANGRY, HAPPY, DEFAULT
  //roboEyes.setPosition(DEFAULT); // cardinal directions, can be N, NE, E, SE, S, SW, W, NW, DEFAULT (default = horizontally and vertically centered)
  //roboEyes.setCuriosity(ON); // bool on/off -> when turned on, height of the outer eyes increases when moving to the very left or very right

  // Set horizontal or vertical flickering
  //roboEyes.setHFlicker(ON, 2); // bool on/off, byte amplitude -> horizontal flicker: alternately displacing the eyes in the defined amplitude in pixels
  //roboEyes.setVFlicker(ON, 2); // bool on/off, byte amplitude -> vertical flicker: alternately displacing the eyes in the defined amplitude in pixels

  // Play prebuilt oneshot animations
  //roboEyes.anim_confused(); // confused - eyes shaking left and right
  //roboEyes.anim_laugh(); // laughing - eyes shaking up and down

  
} // end of setup


void loop() {
  display.setCursor(0,0);
  static uint32_t tmr;
  static bool bState = false;
  static bool flag = true;
  static bool in_eyes = false;
  bool state;
    // таймер
    if (millis() - tmr >= 100) {
        tmr = millis();
        state = !digitalRead(BUTTON_PIN);
        if (state == 1) {  // состояние изменилось
            bState = !bState;     // запомнить новое

        }
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
      state = !digitalRead(BUTTON_PIN);
      if (state == 1){
        bState = !bState;
      }
      if (client.available()) {
        String data = client.readStringUntil('\n');
        data.trim();

        if (data == "1" && !in_eyes){
            digitalWrite(LED_PIN, HIGH);
        }else if (data == "0" && !in_eyes){
            digitalWrite(LED_PIN, LOW);
        }else if (data == "eyes"){
          in_eyes = true;
          client.println("In eyes");
          display.clearDisplay();
          timer = millis();
          while(in_eyes){
            state = !digitalRead(BUTTON_PIN);
            if (state == 1){
              bState = !bState;
            }
            String data = readLineWithTimeout(client, 10);
            data.trim();
            if (data == "exit")in_eyes = false;
            eyes(data);
            if (bState == 0) break;
          }
          
        }else if (data == "exit"){
          in_eyes = false;
        }
        
        // Ответ клиенту
        client.print("ESP32 получил: ");
        client.println(data);
      }
      if (bState == 0){
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
