#define LED1 23
#define NUT 22
#define LED2 14
#define LED3 27
bool bstatus = 0;
unsigned long bt = 0;
void setup(){
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(NUT, INPUT_PULLUP);
  bt = millis();
}
void loop(){
  
  while(digitalRead(NUT) == 0){
    if(millis() - bt > 100)  {
      bstatus = bstatus + 1;
    }
    bt = millis();
  }
  
  if(bstatus % 3 == 0 ){
     digitalWrite(LED1,LOW);
     delay(300);
     digitalWrite(LED2,LOW);
     delay(300);
     digitalWrite(LED3,LOW);
     delay(300);
  }
  if(bstatus % 3 == 1){
    digitalWrite(LED1,1);
    delay(300);
    digitalWrite(LED2,1);
    delay(300);
    digitalWrite(LED3,1);
    
  }
  if(bstatus % 3 == 2){
    digitalWrite(LED1,1);
    digitalWrite(LED1,0);
    digitalWrite(LED1,0);
    delay(300);
    digitalWrite(LED1,0);
    digitalWrite(LED1,1);
    digitalWrite(LED1,0);
    delay(300);
    digitalWrite(LED1,0);
    digitalWrite(LED1,0);
    digitalWrite(LED1,1);

  }
  
 Serial.println(bstatus);
  
}