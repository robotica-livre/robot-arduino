/** Robot Arduino
  * 
  */

// Motores
#define E1 5    //M1 Speed Control
#define E2 6    //M2 Speed Control
#define M1 4    //M1 Direction Control
#define M2 7    //M2 Direction Control
 
// Sensores
#define S1  8
#define S2  9
#define S3 10
#define S4 11

void drive(char a, char b, boolean c, boolean d) {
}

void stop(void) {
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW);      
}

void advance(char a,char b) {
  analogWrite (E1,a);      //PWM Speed Control
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}  

void back_off(char a,char b) {
  analogWrite (E1,a);
  digitalWrite(M1,LOW);   
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}

void turn_L(char a,char b) {
  analogWrite (E1,a);
  digitalWrite(M1,LOW);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}

void turn_R (char a,char b) {
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,LOW);
}

void setup(void) { 
  int i;
  for(i=M1;i<=M2;i++) // Setando saida motores
    pinMode(i, OUTPUT);
  for(i=S1;i<=S4;i++) // Setando entrada sensores
    pinMode(i, INPUT);  
  Serial.begin(19200);      //Set Baud Rate
  Serial.println("Run keyboard control");
}

void loop(void) {
  if(Serial.available()){
    char val = Serial.read();
    if(val != -1) {
      switch(val) {
      case 'w'://Move Forward
        advance (255,255);   //move forward in max speed
        break;
      case 's'://Move Backward
        back_off (255,255);   //move back in max speed
        break;
      case 'a'://Turn Left
        turn_L (100,100);
        break;       
      case 'd'://Turn Right
        turn_R (100,100);
        break;
      case 'z':
        Serial.println("Hello");
        break;
      case 'x':
        stop();
        break;
      }
    }
    else stop();  
  }
  
}
