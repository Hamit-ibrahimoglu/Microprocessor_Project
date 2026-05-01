//Pins Configure
const int tempPins[4] = {A0, A1, A2, A3};
const int motorPins[4] = {3, 5, 6, 9};

void setup() {
  Serial.begin(9600);
  
  //motor pin
  for(int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
}

void loop() {
  // Read the values from LM35 sensors
  int rawTemp1 = analogRead(tempPins[0]);
  int rawTemp2 = analogRead(tempPins[1]);
  int rawTemp3 = analogRead(tempPins[2]);
  int rawTemp4 = analogRead(tempPins[3]);
  
  delay(500);
}
