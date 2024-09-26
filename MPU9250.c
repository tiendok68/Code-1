#include <MPU9250_asukiaaa.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
// kết nối wifi cho mqtt
const char *ssid = "Galaxy Note10 Plus";
const char *password = "16112005";
// cài đặt sever mqtt broker
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
// TOPIC
#define MQTT_TOPIC "dev01/valve"

WiFiClient wifiClient;
PubSubClient client(wifiClient);
// hÀM KẾT NỐI WIFI
void setup_wifi()
{
  Serial.print("connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("...");
  }
  Serial.print("WIFI CONNECTED");
}
// kẾT NỐI VỚI BROKER
void connect_to_broker()
{
  while ((!client.connected()))
  {
    Serial.println("MQTT connection");
    String clientId = "esp32";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.subscribe(MQTT_TOPIC);
    }
    else
    {
      Serial.print("failed");
      delay(2000);
    }
  }
}
// nhận dữ liệu từ broker và thực hiện
void callback1(char *topic, byte *payload, unsigned int length)
{
  char status[20];
  for (int i = 0; i < length; i++)
  {
    status[i] = 20;
  }
  for (int i = 0; i < length; i++)
  {
    status[i] = payload[i];
  }
  if (String(topic) == "dev01/valve")
  {
    if (String(status) == "gui_gX")
    {
      uint8_t sensorId;
      int result;

      result = mySensor.readId(&sensorId);
      if (result == 0)
      {
        Serial.println("sensorId: " + String(sensorId));
      }
      else
      {
        Serial.println("Cannot read sensorId " + String(result));
      }
      /*
        result = mySensor.accelUpdate();
        if (result == 0) {
          aX = mySensor.accelX();
          aY = mySensor.accelY();
          aZ = mySensor.accelZ();
          aSqrt = mySensor.accelSqrt();
          Serial.println("accelX: " + String(aX));
          Serial.println("accelY: " + String(aY));
          Serial.println("accelZ: " + String(aZ));
          Serial.println("accelSqrt: " + String(aSqrt));
        } else {
          Serial.println("Cannod read accel values " + String(result));
        }
       */
      result = mySensor.gyroUpdate();
      if (result == 0)
      {
        gX = mySensor.gyroX();
        gY = mySensor.gyroY();
        gZ = mySensor.gyroZ();
        Serial.println("gyroX: " + String(gX));
        Serial.println("gyroY: " + String(gY));
        Serial.println("gyroZ: " + String(gZ));
        char a[10], b[10], c[10];
        dtostrf(gX, 3, 2, a);
        dtostrf(gX, 3, 2, b);
        dtostrf(gX, 3, 2, c);
        client.publish("dev01/valve", a);
        delay(1000);
      }
      else
      {
        Serial.println("Cannot read gyro values " + String(result));
      }

      /*
      result = mySensor.magUpdate();
      if (result != 0) {
        Serial.println("cannot read mag so call begin again");
        mySensor.beginMag();
        result = mySensor.magUpdate();
      }
      if (result == 0) {
        mX = mySensor.magX();
        mY = mySensor.magY();
        mZ = mySensor.magZ();
        mDirection = mySensor.magHorizDirection();
        Serial.println("magX: " + String(mX));
        Serial.println("maxY: " + String(mY));
        Serial.println("magZ: " + String(mZ));
        Serial.println("horizontal direction: " + String(mDirection));
      } else {
        Serial.println("Cannot read mag values " + String(result));
      }
     */
      Serial.println("at " + String(millis()) + "ms");
      Serial.println(""); // Add an empty line
      delay(500);
    }
  }
}
void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(500);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback1);
  connect_to_broker();
  while (!Serial)
    ;
  Serial.println("started");

#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
#endif

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

  // You can set your own offset for mag values
  // mySensor.magXOffset = -50;
  // mySensor.magYOffset = -55;
  // mySensor.magZOffset = -10;
}

void loop()
{
  client.loop();
  if (!client.connected())
  {
    connect_to_broker();
  }
}