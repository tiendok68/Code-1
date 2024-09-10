#include <HardwareSerial.h>
#define NUT 23
bool x = 1;
int i = millis();
HardwareSerial SerialPort(2);

void setup()  
{
  Serial.begin(9600);
  SerialPort.begin(9600, SERIAL_8N1, 16, 17); 
  pinMode(NUT, INPUT_PULLUP);
  i = millis();
}
void loop(){
 int value = digitalRead(NUT);
 if(value == 0 ){
  if(millis() - i > 250){
      x = !x;
  }
    i = millis();
 }
 if(x == 0){
  Serial.print(1);
  SerialPort.print(1);
  delay(200);
 }else{
  Serial.print(0);
  SerialPort.print(0);
  delay(200);
 }
  
}