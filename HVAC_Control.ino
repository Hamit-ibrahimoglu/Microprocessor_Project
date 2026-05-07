#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int tPins[] = {A0, A1, A2, A3};
int mPins[] = {3, 5, 6, 9};
int emBtn = 2;
int tTemp = 24; 
float corr = 0.5; 

volatile bool stopSys = false; 

void halt_sys() {
  stopSys = true;
}

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 4; i++) {
    pinMode(mPins[i], OUTPUT);
  }
  pinMode(emBtn, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(emBtn), halt_sys, FALLING);
  
  lcd.init();
  lcd.backlight();
}

float get_temp(int pin) {
  analogRead(pin); 
  delay(10);       
  int val = analogRead(pin); 
  float v = (val * 5.0) / 1024.0;
  return v * 100.0; 
}

int get_pwm(float t) {
  if (t <= tTemp) return 0; 
  int spd = (t - tTemp) * 15; 
  if (spd > 255) spd = 255;
  return spd;
}

void print_room(int idx, int c, int r, float t, int p) {
  lcd.setCursor(c, r);
  lcd.print("T");
  lcd.print(idx + 1); 
  lcd.print(":"); 
  
  int ti = (int)t;
  if (ti < 10 && ti >= 0) lcd.print("0"); 
  lcd.print(ti);
  
  lcd.print("F"); 
  
  if (p <= 0) {
    lcd.print("00");
  } else if (p >= 100) {
    lcd.print("MX");
  } else {
    if (p < 10) lcd.print("0");
    lcd.print(p);
  }
}

void loop() {
  if (stopSys) {
    for(int i = 0; i < 4; i++) {
      analogWrite(mPins[i], 0); 
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" SYSTEM HALTED! ");
    lcd.setCursor(0, 1);
    lcd.print(" HARDWARE RESET ");
    Serial.println("EMERGENCY STOP!");
    while(true); 
  }

  float t1 = get_temp(tPins[0]);
  float t2 = get_temp(tPins[1]);
  float t3 = get_temp(tPins[2]);
  float t4 = get_temp(tPins[3]);
  
  int b1 = get_pwm(t1);
  int b2 = get_pwm(t2);
  int b3 = get_pwm(t3);
  int b4 = get_pwm(t4);

  int f1 = b1 + (corr * b2);
  int f2 = b2 + (corr * b1) + (corr * b3);
  int f3 = b3 + (corr * b2) + (corr * b4);
  int f4 = b4 + (corr * b3);

  if (f1 > 255) f1 = 255;
  if (f2 > 255) f2 = 255;
  if (f3 > 255) f3 = 255;
  if (f4 > 255) f4 = 255;

  analogWrite(mPins[0], f1);
  analogWrite(mPins[1], f2);
  analogWrite(mPins[2], f3);
  analogWrite(mPins[3], f4);
  
  int p1 = map(f1, 0, 255, 0, 100);
  int p2 = map(f2, 0, 255, 0, 100);
  int p3 = map(f3, 0, 255, 0, 100);
  int p4 = map(f4, 0, 255, 0, 100);

  print_room(0, 0, 0, t1, p1); 
  print_room(1, 8, 0, t2, p2); 
  print_room(2, 0, 1, t3, p3); 
  print_room(3, 8, 1, t4, p4); 
  
  Serial.print("T1:"); Serial.print((int)t1); Serial.print("C F1:"); Serial.print(p1); Serial.println("%");
  Serial.print("T2:"); Serial.print((int)t2); Serial.print("C F2:"); Serial.print(p2); Serial.println("%");
  Serial.print("T3:"); Serial.print((int)t3); Serial.print("C F3:"); Serial.print(p3); Serial.println("%");
  Serial.print("T4:"); Serial.print((int)t4); Serial.print("C F4:"); Serial.print(p4); Serial.println("%");
  Serial.println("-----------------");

  delay(400); 
}
