#include <Arduino.h>
String text;
void setup(){
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(22,LOW);
  digitalWrite(23,LOW);
  Serial.println("BAN HAY CHON KEO HOAC BUA HOAC BAO");
  srand((int)time(0));
}
void loop(){
  int a = rand() % 3;
  String maychon;
  if(a == 1){
    maychon = "KEO";
  }else if(a == 2){
    maychon = "BUA";
  }else if(a == 0){
    maychon = "BAO";
  }
  if(Serial.available() > 0){
    text = Serial.readStringUntil('\n');
    if(text == "KEO"){
      Serial.println("Ban da chon KEO");
      if(maychon == "KEO"){
        Serial.print("May chon KEO nen hoa, den vang sang");
        digitalWrite(2,HIGH);
      }
      if(maychon == "BUA"){
        Serial.println("May chon BUA nen ban thua, den do sang");
        digitalWrite(22,HIGH);
      }
      if(maychon == "BAO"){
        Serial.println("May chon BAO nen ban win, den xanh sang");
        digitalWrite(23,HIGH);
      }
    }
    if(text == "BUA"){
      Serial.println("Ban da chon BUA");
      if(maychon == "KEO"){
        Serial.println("May chon KEO nen ban win, den xanh sang");
        digitalWrite(23,HIGH);
      }
      if(maychon == "BUA"){
        Serial.println("May chon BUA nen hoa, den vang sang");
        digitalWrite(2,HIGH);
      }
      if(maychon == "BAO"){
        Serial.println("May chon BAO nen ban thua, den do sang");
        digitalWrite(22,HIGH);
      }

    }
    if(text == "BAO"){
      Serial.println("Ban da chon BAO");
      if(maychon == "KEO"){
        Serial.println("May chon KEO nen ban thua, den do sang");
        digitalWrite(22,HIGH);
      }
      if(maychon == "BUA"){
        Serial.println("May chon BUA nen thang, den xanh sang");
        digitalWrite(23,HIGH);
      }
      if(maychon == "BAO"){
        Serial.println("May chon BAO nen hoa, den vang sang");
        digitalWrite(2,HIGH);
      }
      
    }
  }
  delay(3000);
  digitalWrite(2,LOW);
  digitalWrite(22,LOW);
  digitalWrite(23,LOW);
}