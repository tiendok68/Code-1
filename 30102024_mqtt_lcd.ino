#include <WiFi.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>
#include <TFT_eSPI.h>  // Thư viện TFT
#include <SPI.h>
// Thông tin WiFi
const char* ssid = "Galaxy Note10 Plus";
const char* password = "16112005";

// MQTT Broker
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
unsigned long previousMillis = 0,interval = 5000;

#define MQTT_LED1_TOPIC "ONG _VANG/LED" // Đổi thành tên topic bạn muốn sử dụng

WiFiClient wifiClient;
PubSubClient client(wifiClient);

TFT_eSPI tft = TFT_eSPI();  // Khởi tạo đối tượng màn hình TFT
void setup_wifi(){
  Serial.print("connecting");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("...");
  }
  Serial.print("WIFI CONNECTED");
  
 }
 void callback(char* topic, byte* message, unsigned int length) {
  
  String receivedMessage;
  for (int i = 0; i < length; i++) {
    receivedMessage += (char)message[i];
  }
   Serial.println(receivedMessage);
  
  
  int x, y;

// Tìm vị trí của khoảng trắng trong chuỗi
int spaceIndex = receivedMessage.indexOf(' ');

// Tách phần trước khoảng trắng và chuyển thành số nguyên cho x
x = receivedMessage.substring(0, spaceIndex).toInt();

// Tách phần sau khoảng trắng và chuyển thành số nguyên cho y
y = receivedMessage.substring(spaceIndex + 1).toInt();

Serial.print("x = ");
Serial.println(x);
Serial.print("y = ");
Serial.println(y);
tft.fillCircle(x, y, 1, TFT_RED); 
  // Kiểm tra nếu tin nhắn là "0"
  if (receivedMessage == "0") {
    Serial.println("Nhận giá trị 0");
  }
}
  
void connect_to_broker(){
  while((!client.connected())){
    Serial.println("MQTT connection");
    String clientId = "esp32";
    clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str())){
      Serial.println("connected");
      client.subscribe(MQTT_LED1_TOPIC);
      
    }else{
      Serial.print("failed");
      delay(2000);
    }
  }
}
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(500);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  connect_to_broker();
  client.setCallback(callback);

  tft.init();                 // Khởi động màn hình
  tft.setRotation(1);         // Xoay màn hình (1)
  tft.fillScreen(TFT_BLACK);  // Tô nền màu đen
  drawArrow(20, 120, 300, 120, TFT_WHITE);  // Trục X ngang
  drawArrow(160, 220, 160, 0, TFT_WHITE);   // Trục Y dọc
  // Viết điểm O
  tft.setCursor(140, 130);
  tft.setTextSize(2);
  tft.println('O');
}
void loop() {
   
   if(!client.connected()){
    connect_to_broker();
  }
  client.loop();
  if (Serial.available() > 0) {
    Serial.print(1);
  }



}
void drawArrow(int x1, int y1, int x2, int y2, uint16_t color) {
  // Vẽ đường thẳng trục chính
  tft.drawLine(x1, y1, x2, y2, color);

  // Tính toán hướng mũi tên
  int arrowSize = 10;  // Kích thước của mũi tên

  // Nếu là trục X (nằm ngang)
  if (y1 == y2) {
    if (x2 > x1) {  // Mũi tên hướng phải
      tft.fillTriangle(x2, y2, x2 - arrowSize, y2 - arrowSize / 2, x2 - arrowSize, y2 + arrowSize / 2, color);
    } else {  // Mũi tên hướng trái
      tft.fillTriangle(x2, y2, x2 + arrowSize, y2 - arrowSize / 2, x2 + arrowSize, y2 + arrowSize / 2, color);
    }
  }

  // Nếu là trục Y (thẳng đứng)
  if (x1 == x2) {
    if (y2 < y1) {  // Mũi tên hướng lên
      tft.fillTriangle(x2, y2, x2 - arrowSize / 2, y2 + arrowSize, x2 + arrowSize / 2, y2 + arrowSize, color);
    } else {  // Mũi tên hướng xuống
      tft.fillTriangle(x2, y2, x2 - arrowSize / 2, y2 - arrowSize, x2 + arrowSize / 2, y2 - arrowSize, color);
    }
  }
}