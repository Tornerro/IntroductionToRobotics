const int blueLedPin = 11;
const int greenLedPin = 10;
const int redLedPin = 9;

const int bluePotPin = A0;
const int greenPotPin = A1;
const int redPotPin = A2;

const int maxAnalogValue = 1023;
const int maxVoltageValue = 255;

int bluePotValue = 0;
int greenPotValue = 0;
int redPotValue = 0;

int blueLedBrightness = 0;
int greenLedBrightness = 0;
int redLedBrightness = 0;

#define ZERO 0

void setup() {
  pinMode (blueLedPin, OUTPUT);
  pinMode (greenLedPin, OUTPUT);
  pinMode (redLedPin, OUTPUT);
  
  pinMode (bluePotPin, INPUT);
  pinMode (greenPotPin, INPUT);
  pinMode (redPotPin, INPUT);
  
  Serial.begin (9600);
}

void loop() { // I read the values from each potentiometer, map the values in the interval [0, 255] and send the corresponding voltage to each pin, and write in the serial the R G B values
  

  bluePotValue = analogRead (bluePotPin);
  greenPotValue = analogRead (greenPotPin);
  redPotValue = analogRead (redPotPin);

  blueLedBrightness = map (bluePotValue, ZERO, maxAnalogValue, ZERO, maxVoltageValue);
  greenLedBrightness = map (greenPotValue, ZERO, maxAnalogValue, ZERO, maxVoltageValue);
  redLedBrightness = map (redPotValue, ZERO, maxAnalogValue, ZERO, maxVoltageValue);
  
  analogWrite (blueLedPin, blueLedBrightness);
  analogWrite (greenLedPin, greenLedBrightness);
  analogWrite (redLedPin, redLedBrightness);

  Serial.print ("R: ");
  Serial.print (redLedBrightness);
  Serial.print ("    G: ");
  Serial.print (greenLedBrightness);
  Serial.print ("    B: ");
  Serial.println(blueLedBrightness);
}
