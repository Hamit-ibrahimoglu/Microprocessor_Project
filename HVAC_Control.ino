#include <LiquidCrystal.h>

// Moved d7 to pin 11 to free up pin 2 for the emergency interrupt
const int rs = 13, en = 12, d4 = 8, d5 = 7, d6 = 4, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int tempPins[4] = {A0, A1, A2, A3};
const int motorPins[4] = {3, 5, 6, 9};

// Emergency button connected to Pin 2 for hardware interrupt support
const int emergencyBtnPin = 2;

const int targetTemp = 24; 
float correlation = 0.5; // Final tuning: increased correlation for faster response

volatile bool isEmergency = false; 

void emergency_isr() {
  isEmergency = true;
}

void setup() {
  Serial.begin(9600);
  
  for(int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  
  pinMode(emergencyBtnPin, INPUT_PULLUP); 
  
  attachInterrupt(digitalPinToInterrupt(emergencyBtnPin), emergency_isr, FALLING);
  
  lcd.begin(16, 2);
}

float readTemp(int pin) {
  int sensorVal = analogRead(pin);
  float voltage = (sensorVal * 5.0) / 1024.0;
  return (voltage - 0.5) * 100.0;
}

int calc_base_pwm(float temp) {
  if (temp <= targetTemp) {
    return 0; 
  }
  int speed = (temp - targetTemp) * 15; 
  if (speed > 255) {
    speed = 255;
  }
  return speed;
}

void displayRoom(int roomIndex, int col, int row, float temp, int speedPercent) {
  lcd.setCursor(col, row);
  lcd.print(roomIndex + 1); 
  lcd.print(":"); 
  
  // Format temperature display
  int tInt = (int)temp;
  if (tInt < 10 && tInt >= 0) lcd.print("0"); 
  lcd.print(tInt);
  lcd.print("C "); 
  
  // Format speed display (00, MX, or 2 digits)
  if (speedPercent <= 0) {
    lcd.print("00 ");
  } else if (speedPercent >= 100) {
    lcd.print("MX ");
  } else {
    if (speedPercent < 10) lcd.print("0");
    lcd.print(speedPercent);
    lcd.print(" ");
  }
}

void loop() {
  if (isEmergency) {
    for(int i = 0; i < 4; i++) {
      analogWrite(motorPins[i], 0); 
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" SYSTEM HALTED! ");
    lcd.setCursor(0, 1);
    lcd.print(" HARDWARE RESET ");
    
    while(true) {} 
  }

  float temp1 = readTemp(tempPins[0]);
  float temp2 = readTemp(tempPins[1]);
  float float temp3 = readTemp(tempPins[2]);
  float temp4 = readTemp(tempPins[3]);
  
  int base1 = calc_base_pwm(temp1);
  int base2 = calc_base_pwm(temp2);
  int base3 = calc_base_pwm(temp3);
  int base4 = calc_base_pwm(temp4);

  int final1 = base1 + (correlation * base2);
  int final2 = base2 + (correlation * base1) + (correlation * base3);
  int final3 = base3 + (correlation * base2) + (correlation * base4);
  int final4 = base4 + (correlation * base3);

  if (final1 > 255) final1 = 255;
  if (final2 > 255) final2 = 255;
  if (final3 > 255) final3 = 255;
  if (final4 > 255) final4 = 255;

  analogWrite(motorPins[0], final1);
  analogWrite(motorPins[1], final2);
  analogWrite(motorPins[2], final3);
  analogWrite(motorPins[3], final4);
  
  int pct1 = map(final1, 0, 255, 0, 100);
  int pct2 = map(final2, 0, 255, 0, 100);
  int pct3 = map(final3, 0, 255, 0, 100);
  int pct4 = map(final4, 0, 255, 0, 100);

  displayRoom(0, 0, 0, temp1, pct1); 
  displayRoom(1, 8, 0, temp2, pct2); 
  displayRoom(2, 0, 1, temp3, pct3); 
  displayRoom(3, 8, 1, temp4, pct4); 
  
  delay(200);
}
