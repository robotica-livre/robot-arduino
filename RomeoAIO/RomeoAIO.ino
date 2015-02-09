/** Robot Arduino
 * 
 */
#include <LiquidCrystal.h>

// Motores
#define M1 4 //M1 Direction Control
#define E1 5 //M1 Speed Control
#define E2 6 //M2 Speed Control
#define M2 7 //M2 Direction Control
 
// Sensores
#define S1  8
#define S2  9
#define S3 10
#define S4 11

// Direcao
#define FORWARD  0
#define BACKWARD 1
#define LEFT     2
#define RIGHT    3

// Display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int state = 0;

void stop(void) {
  digitalWrite(E1, LOW);   
  digitalWrite(E2, LOW);      
}

void drive(char left, char right, int direction) {
  analogWrite (E1, left);
  analogWrite (E2, right);    
  
  switch(direction) {
  case FORWARD:
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    break;
  case BACKWARD:
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    break;
  case LEFT:
    digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
    break;
  case RIGHT:
    digitalWrite(M1, HIGH);
    digitalWrite(M2, LOW);
    break;
  }
}

void printMenu() {
  lcd.setCursor(0,0);
  lcd.print("Robot-arduino");
}

void setup(void) { 
  int i;
  for(i=M1; i<=M2; i++)
    pinMode(i, OUTPUT);
  for(i=S1; i<=S4; i++)
    pinMode(i, INPUT);  
  lcd.begin(0,0); 
}

void loop(void) {
  switch(state) {
  case 0:
      printMenu();
      break;
  }
  
}
