#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16 , 2);
HardwareSerial SerialPort(2); 
#define LED 23

void setup()
{
  Serial.begin(9600);
  SerialPort.begin(9600, SERIAL_8N1, 16, 17);
  pinMode(LED, OUTPUT);
  lcd.init();
  lcd.backlight();
}
void loop()
{    
  if(SerialPort.available()){
    int x = SerialPort.read();
    Serial.print(x);
    if(x == 49){
      digitalWrite(LED, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("den sang");
      delay(100);
    }else{
      digitalWrite(LED, LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("den tat");
      delay(100);
    }
  }
}