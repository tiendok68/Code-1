#include<WiFi.h>
#include<PubSubClient.h>
#include<ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

DHT dht(12, DHT11);

#define LED1 17
#define LED2 5
String ledstatus1 = "ON";
String ledstatus2 = "ON";
const char*ssid = "Tien Do";
const char*password = "16112005";
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_USER "tiendo"
#define MQTT_PASSWORD "12345678"

#define MQTT_LED1_TOPIC "ONG _VANG/LED1"
#define MQTT_LED2_TOPIC "ONG _VANG/LED2"

unsigned long previousMillis = 0,interval = 5000;

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
  dht.begin();
 }
void connect_to_broker(){
  while((!client.connected())){
    Serial.println("MQTT connection");
    String clientId = "esp32";
    clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str())){
      Serial.println("connected");
      client.subscribe(MQTT_LED1_TOPIC);
      client.subscribe(MQTT_LED2_TOPIC);
    }else{
      Serial.print("failed");
      delay(2000);
    }
  }
}
void callback1(char* topic,byte *payload,unsigned int length){
  char status[20];
  for(int i = 0; i < 20; i ++){
    status[i] = 0;
  }
  for(int i = 0; i < length; i ++){
    status[i] = payload[i];
  }
  if(String(topic) == MQTT_LED1_TOPIC){
    if(String(status) == "OFF"){
      ledstatus1 = "OFF";
      digitalWrite(LED1, LOW);

    }
    if(String(status) == "ON"){
      ledstatus1 = "ON";
      digitalWrite(LED1, HIGH);

    }
  }
  if(String(topic) == MQTT_LED2_TOPIC){
    if(String(status) == "OFF"){
      ledstatus2 = "OFF";
      digitalWrite(LED2, LOW);

    }
    if(String(status) == "ON"){
      ledstatus2 = "ON";
      digitalWrite(LED2, HIGH);

    }
  }

}
void setup(){
  Serial.begin(9600);
  Serial.setTimeout(500);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback1);
  connect_to_broker;
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);


}
void loop(){
  client.loop();
  float h = dht.readHumidity();

  float t = dht.readTemperature();
  char tem[10];
  dtostrf(t,1,2,tem);
  if(!client.connected()){
    connect_to_broker();

  }
  int x = millis();
  if(millis() - x > 2000){
    client.publish("ONG _VANG/LED1",tem);
    x = millis();
  }
}


