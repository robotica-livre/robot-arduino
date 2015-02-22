/** Robot Arduino
 * 
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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

// Definicoes dos botoes
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Display
LiquidCrystal_I2C lcd(32, 16, 2);


int state = 0;
int sel = 0;
int botoes;

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

void lineFollowing() {
  // TODO: Implement Line Following algorithm
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
  lcd.init();
  lcd.backlight();
  attachInterrupt(0, lineFollowing, RISING);
}

void loop(void) {
  atualiza_menu();
  menu_list();
}

void atualiza_menu() {
  if(botoes > 100 && botoes < 200) {
    if(state > 0) {
      state--;
    }
  } else if(botoes > 700 && botoes < 800) {
    if(state < 2) {
      state++;
    }
  } else if(botoes < 100) {
    sel = 1;
  }
}

void menu_list() {
  if(state == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Robot-Arduino");
    lcd.setCursor(0, 1);
    lcd.print("");
  } else if(state == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Ajustar Sensores");
    lcd.setCursor(0, 1);
    lcd.print("");
  } else if(state == 2) {
    lcd.setCursor(0, 0);
    lcd.print(" Seguir Linha ");
    lcd.setCursor(0, 1);
    lcd.print("");
  } else if(state == 3) {
    lcd.setCursor(0, 0);
    lcd.print(" Testar motores ");
    lcd.setCursor(0, 1);
    lcd.print("");
  }
}
