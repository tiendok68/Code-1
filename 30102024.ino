#include <WiFi.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>

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
  Serial.print("Nhận tin nhắn từ topic: ");
  Serial.println(topic);
  
  String receivedMessage;
  for (int i = 0; i < length; i++) {
    receivedMessage += (char)message[i];
  }

  Serial.print("Tin nhắn: ");
  Serial.println(receivedMessage);

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
}
void loop() {
   
   if(!client.connected()){
    connect_to_broker();
  }
  client.loop();
}
