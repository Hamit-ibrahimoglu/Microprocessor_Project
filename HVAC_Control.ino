#include <LiquidCrystal.h>

// Setup pins for the LCD
const int rs = 13, en = 12, d4 = 8, d5 = 7, d6 = 4, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define Pins for temp sensors and motors
const int tempPins[4] = {A0, A1, A2, A3};
const int motorPins[4] = {3, 5, 6, 9};

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

// Function to place and print each room's info on the screen
void displayRoom(int roomIndex, int col, int row, float temp, int speedPercent) {
  lcd.setCursor(col, row);
  lcd.print(roomIndex + 1); 
  lcd.print(":"); 
  lcd.print((int)temp); 
  lcd.print("C "); 
  lcd.print(speedPercent); 
  lcd.print("%"); 
}

void loop() {
  // Read and calculate temperature for all 4 room's
  float temp1 = readTemp(tempPins[0]);
  float temp2 = readTemp(tempPins[1]);
  float temp3 = readTemp(tempPins[2]);
  float temp4 = readTemp(tempPins[3]);
  
  // Show data on LCD (motor speed is 0 for now until I add PWM code)
  displayRoom(0, 0, 0, temp1, 0); 
  displayRoom(1, 8, 0, temp2, 0); 
  displayRoom(2, 0, 1, temp3, 0); 
  displayRoom(3, 8, 1, temp4, 0); 
  
  delay(200);
}
