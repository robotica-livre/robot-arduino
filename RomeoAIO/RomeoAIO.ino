 /**
 * Robot Arduino
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MenuSystem.h>

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

// Menu dos Display
MenuSystem ms;
Menu mm("Robot Arduino");
Menu ajustarParametros("Ajustar Parametros");
MenuItem seguirLinha("Seguir Linha");
Menu diagnostico("Diagnostico");
MenuItem testarMotores("Testar Motores");
MenuItem testarSensores("Testar Sensores");

int adc_key_val[5] = { 30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;

float average = 0;
int last_proportional;
int integral;

void stop(void) {
  digitalWrite(E1, LOW);
  digitalWrite(E2, LOW);
}

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
  }
}

void lineFollowing(MenuItem* p_menu_item) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Go! go! go! ...");
  delay(500);
  drive(255,255,FORWARD);
  while (1) {
    unsigned int position = read_sensor_data(); // TODO: Read sensor data

    int proportional = ((int)position) - 2000;

    int derivative = proportional - last_proportional;
    integral += proportional;

    last_proportional = proportional;

    int power_difference = proportional / 10 + integral / 10000 + derivative * 3 / 2;

    const int max = 180;
    if (power_difference > max)
      power_difference = max;
    if (power_difference < -max)
      power_difference = -max;
    if (power_difference < 0)
      drive(max + power_difference, max, FORWARD);
    else
      drive(max, max - power_difference, FORWARD);
    
    adc_key_in = analogRead(0);
    if(get_key(adc_key_in) == 0) { stop(); return; }
  }
}

int read_sensor_data() {
  byte sens = (digitalRead(S1) == LOW) + (digitalRead(S2) == LOW) * 2 + (digitalRead(S3) == LOW) * 4 + (digitalRead(S4) == LOW) * 8;
  switch(sens) {
    case B00000001:
      return 2;
    case B00000010:
      return 1;
    case B00000110:
      return 0;
    case B00000100:
      return -1;
    case B00001000:
      return -2;
    default:
      return 0;
  }
}

void testa_motores(MenuItem* p_menu_item) {
  drive(255, 255, FORWARD);
  delay(2000);
  drive(255, 255, BACKWARD);
  delay(2000);
  drive(255, 255, LEFT);
  delay(2000);
  drive(255, 255, RIGHT);
  delay(2000);
  stop();
}

void testa_sensores(MenuItem* p_menu_item) {
  lcd.clear();
  lcd.setCursor(0,1);
  while(1) {
    lcd.clear();
    lcd.print(read_sensor_data());
    delay(500);
    adc_key_in = analogRead(0);
    if(get_key(adc_key_in) == 0) return;
  }
}

void atualiza_menu() {
  lcd.setCursor(0,0);
  Menu const* cp_menu = ms.get_current_menu();
  lcd.print(cp_menu->get_name());
  lcd.setCursor(0,1);
  lcd.print(cp_menu->get_selected()->get_name());
  
  adc_key_in = analogRead(0);
  int key = get_key(adc_key_in);
  switch (key) {
    case 0: lcd.clear(); ms.back(); delay(500); break;
    case 1: lcd.clear(); ms.prev(); delay(500); break;
    case 3: lcd.clear(); ms.next(); delay(500); break;
    case 4: lcd.clear(); ms.select(); delay(500); break;
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
