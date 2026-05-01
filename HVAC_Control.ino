//Pins Configure
const int tempPins[4] = {A0, A1, A2, A3};
const int motorPins[4] = {3, 5, 6, 9};

void setup() {
  Serial.begin(9600);
  
  //motor pins setup
  for(int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
}


//ADC value to Celsius (Signal Scaling) convert Function
float readTemp(int pin) {
  int sensorVal = analogRead(pin);
  //conversion: 5V / 1024 steps = 4.88mV per step
  float voltage = (sensorVal * 5.0) / 1024.0;
  return (voltage - 0.5) * 100.0;
}


void loop() {
  //processe readings through the scaling function
  float temp1 = readTemp(tempPins[0]);
  float temp2 = readTemp(tempPins[1]);
  float temp3 = readTemp(tempPins[2]);
  float temp4 = readTemp(tempPins[3]);
  
  delay(500);
}
