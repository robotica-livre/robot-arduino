/**
 * Robot Arduino
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MenuSystem.h>
#include <EEPROM.h>

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
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Menu do Display
MenuSystem ms;
Menu mm("Robot Arduino");
Menu ajustarParametros("Ajustar Parametros");
MenuItem parametroKp("Parametrizar Kp");
MenuItem parametroKi("Parametrizar Ki");
MenuItem parametroKd("Parametrizar Kd");
MenuItem seguirLinha("Seguir Linha");
Menu diagnostico("Diagnostico");
MenuItem testarMotores("Testar Motores");
MenuItem testarSensores("Testar Sensores");

int adc_key_val[5] = { 50, 200, 400, 600, 800 };
int NUM_KEYS = 5;
int adc_key_in;

float average = 0;
double prev_error = 0;
double integral = 0;
int Kp = 20;
int Ki = 10000;
double Kd = 2;

void drive(char left, char right, int direction) {
  analogWrite (E1, left);
  analogWrite (E2, right);

  switch (direction) {
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
    default:
      digitalWrite(E1, LOW);
      digitalWrite(E2, LOW);
  }
}

void parametrizar_Kp(MenuItem* p_menu_item) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Kp");
  while(1) {
    lcd.setCursor(0,1);
    String msg = "1 / ";
    lcd.print("1 / " + String(Kp));
    switch(get_key(analogRead(7))) {
      case 1: Kp -= 10; break;
      case 3: Kp += 10; break;
      case 0: return;
    }
    delay(200);
  }
}

void parametrizar_Ki(MenuItem* p_menu_item) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ki");
  int value = Ki;
  while(1) {
    lcd.setCursor(0,1);
    lcd.print("1 / " + String(Ki));
    switch(get_key(analogRead(7))) {
      case 1: Ki -= 1000; break;
      case 3: Ki += 1000; break;
      case 0: return;
    }
    delay(200);
  }
}

void parametrizar_Kd(MenuItem* p_menu_item) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Kd");
  while(1) {
    lcd.setCursor(0,1);
    lcd.print("3 / " + String(Kd));
    switch(get_key(analogRead(7))) {
      case 1: Kd -= 0.1; break;
      case 3: Kd += 0.1; break;
      case 0: return;
    }
    delay(200);
  }
}

void lineFollowing(MenuItem* p_menu_item) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Go! go! go! ...");
  delay(500);
  drive(255,255,FORWARD);
  while (1) {
    delay(100);
    double position = read_sensor_data();
    
    double error = 0 - position;
    double integral = integral + error * 0.1;
    double derivative = (error - prev_error) / 0.1;

    double power_difference  = ((1 / Kp) * error) + ((1 / Ki) * integral) + ((3 / Kd) * derivative);
    prev_error = error;

    const int max = 180;
    if (power_difference > max)
      power_difference = max;
    if (power_difference < -max)
      power_difference = -max;
    if (power_difference < 0)
      drive(max + power_difference, max, FORWARD);
    else
      drive(max, max - power_difference, FORWARD);
    
    if(get_key(analogRead(7)) == 0) { drive(180, 180, -1); return; }
  }
}

double read_sensor_data() {
  byte sens = (digitalRead(S1) == HIGH) + (digitalRead(S2) == HIGH) * 2 + (digitalRead(S3) == HIGH) * 4 + (digitalRead(S4) == HIGH) * 8;
  switch(sens) {
    case B00000001:
      return 180;
    case B00000010:
      return 90;
    case B00000110:
      return 0;
    case B00000100:
      return -90;
    case B00001000:
      return -180;
    default:
      return 0;
  }
}

void testa_motores(MenuItem* p_menu_item) {
  drive(180, 180, FORWARD);
  delay(2000);
  drive(180, 180, BACKWARD);
  delay(2000);
  drive(180, 180, LEFT);
  delay(2000);
  drive(180, 180, RIGHT);
  delay(2000);
  drive(180, 180, -1);
}

void testa_sensores(MenuItem* p_menu_item) {
  lcd.setCursor(0,1);
  while(1) {
    lcd.clear();
    lcd.print(read_sensor_data());
    delay(500);
    if(get_key(analogRead(7)) == 0) return;
  }
}

void atualiza_menu() {
  lcd.setCursor(0,0);
  Menu const* cp_menu = ms.get_current_menu();
  lcd.print(cp_menu->get_name());
  lcd.setCursor(0,1);
  lcd.print(cp_menu->get_selected()->get_name());
  
  adc_key_in = analogRead(7);
  int key = get_key(adc_key_in);
  switch (key) {
    case 0: lcd.clear(); ms.back();   delay(200); break;
    case 1: lcd.clear(); ms.prev();   delay(200); break;
    case 3: lcd.clear(); ms.next();   delay(200); break;
    case 4: lcd.clear(); ms.select(); delay(200); break;
  }
}

// Convert ADC value to key number
int get_key(unsigned int input) {
  int k;
  for (k = 0; k < NUM_KEYS; k++) {
    if (input < adc_key_val[k]) return k;
  }
  if (k >= NUM_KEYS) k = -1; // No valid key pressed
  return k;
}

void setup(void) {
  int i;
  for (i = M1; i <= M2; i++)
    pinMode(i, OUTPUT);
  for (i = S1; i <= S4; i++)
    pinMode(i, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  mm.add_menu(&ajustarParametros);
  ajustarParametros.add_item(&parametroKp, parametrizar_Kp);
  ajustarParametros.add_item(&parametroKi, parametrizar_Ki);
  ajustarParametros.add_item(&parametroKd, parametrizar_Kd);
  mm.add_item(&seguirLinha, &lineFollowing);
  mm.add_menu(&diagnostico);
  diagnostico.add_item(&testarMotores, &testa_motores);
  diagnostico.add_item(&testarSensores, &testa_sensores);
  ms.set_root_menu(&mm);
  atualiza_menu();
}

void loop(void) {
  atualiza_menu();
}
