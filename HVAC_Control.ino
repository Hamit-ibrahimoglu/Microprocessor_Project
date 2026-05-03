#include <LiquidCrystal.h>

// Setup pins for the LCD
const int rs = 13, en = 12, d4 = 8, d5 = 7, d6 = 4, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define Pins for temp sensors and motors
const int tempPins[4] = {A0, A1, A2, A3};
const int motorPins[4] = {3, 5, 6, 9};

// System parameters
const int targetTemp = 24; // Target room temperature
float correlation = 0.3; // Cross-coupling factor between rooms

void setup() {
  Serial.begin(9600);
  
  for(int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  
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
  lcd.print((int)temp); 
  lcd.print("C "); 
  lcd.print(speedPercent); 
  lcd.print("% "); 
}

void loop() {
  float temp1 = readTemp(tempPins[0]);
  float temp2 = readTemp(tempPins[1]);
  float temp3 = readTemp(tempPins[2]);
  float temp4 = readTemp(tempPins[3]);
  
  // Calculate base speeds
  int base1 = calc_base_pwm(temp1);
  int base2 = calc_base_pwm(temp2);
  int base3 = calc_base_pwm(temp3);
  int base4 = calc_base_pwm(temp4);

  // Apply Cross-coupling correlation between adjacent rooms
  int final1 = base1 + (correlation * base2);
  int final2 = base2 + (correlation * base1) + (correlation * base3);
  int final3 = base3 + (correlation * base2) + (correlation * base4);
  int final4 = base4 + (correlation * base3);

  // FIX: Added limits to prevent PWM overflow from cross-coupling
  if (final1 > 255) final1 = 255;
  if (final2 > 255) final2 = 255;
  if (final3 > 255) final3 = 255;
  if (final4 > 255) final4 = 255;

  analogWrite(motorPins[0], final1);
  analogWrite(motorPins[1], final2);
  analogWrite(motorPins[2], final3);
  analogWrite(motorPins[3], final4);
  
  // Convert PWM to Percentage
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
