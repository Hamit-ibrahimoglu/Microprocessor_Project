#include <LiquidCrystal.h>

// Setup pins for the LCD
const int rs = 13, en = 12, d4 = 8, d5 = 7, d6 = 4, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define Pins for temp sensors and motors
const int tempPins[4] = {A0, A1, A2, A3};
const int motorPins[4] = {3, 5, 6, 9};

// System parameters
const int targetTemp = 24; // Target room temperature

void setup() {
  Serial.begin(9600);
  
  // Set motor pins as outputs
  for(int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  
  // Start the 16x2 LCD
  lcd.begin(16, 2);
}

//Function to convert raw sensor reading into Celsius
float readTemp(int pin) {
  int sensorVal = analogRead(pin);
  // conversion: 5V / 1024 steps = 4.88mV per step
  float voltage = (sensorVal * 5.0) / 1024.0;
  return (voltage - 0.5) * 100.0;
}

// Function to calculate base motor speed (PWM) based on temperature
int calc_base_pwm(float temp) {
  // If room is cooler than target, turn off fan
  if (temp <= targetTemp) {
    return 0; 
  }
  // Increase speed as it gets hotter
  int speed = (temp - targetTemp) * 15; 
  
  // Limit max PWM to 255 (Arduino Uno max limit)
  if (speed > 255) {
    speed = 255;
  }
  return speed;
}

// Function to place and print each room's info on the screen
void displayRoom(int roomIndex, int col, int row, float temp, int speedPercent) {
  lcd.setCursor(col, row);
  lcd.print(roomIndex + 1); 
  lcd.print(":"); 
  lcd.print((int)temp); 
  lcd.print("C "); 
  lcd.print(speedPercent); 
  lcd.print("% "); // Added extra space to clear old numbers if speed drops
}

void loop() {
  // Read and calculate temperature for all 4 room's
  float temp1 = readTemp(tempPins[0]);
  float temp2 = readTemp(tempPins[1]);
  float temp3 = readTemp(tempPins[2]);
  float temp4 = readTemp(tempPins[3]);
  
  // Calculate base speeds for each room (0 to 255)
  int base1 = calc_base_pwm(temp1);
  int base2 = calc_base_pwm(temp2);
  int base3 = calc_base_pwm(temp3);
  int base4 = calc_base_pwm(temp4);

  // Send PWM signals to motors
  analogWrite(motorPins[0], base1);
  analogWrite(motorPins[1], base2);
  analogWrite(motorPins[2], base3);
  analogWrite(motorPins[3], base4);
  
  // Convert PWM (0-255) to Percentage (0-100) for display
  int pct1 = map(base1, 0, 255, 0, 100);
  int pct2 = map(base2, 0, 255, 0, 100);
  int pct3 = map(base3, 0, 255, 0, 100);
  int pct4 = map(base4, 0, 255, 0, 100);

  // Show real data on LCD
  displayRoom(0, 0, 0, temp1, pct1); 
  displayRoom(1, 8, 0, temp2, pct2); 
  displayRoom(2, 0, 1, temp3, pct3); 
  displayRoom(3, 8, 1, temp4, pct4); 
  
  delay(200);
}
