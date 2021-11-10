#define NUMREADINGS 15

const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;
const int pinDP = 9;

const int buzzerPin = 10;
const int buzzerTone = 100;

const int antennaPin = A1;

const int maxAnalogVal = 1023;

const int segSize = 8;
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

bool dpState = 0;
const int noOfDigits = 10;

int digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

int readings[NUMREADINGS];
int index = 0;
int total = 0;
int average = 0;
int senseLimit = 20;
int val = 0;

int updateTime = 200;
long long lastUpdateTime = 0;


void displayNumber (int digit, bool decimalPoint){
  for (int i = 0; i < segSize - 1; i++){
    digitalWrite (segments[i], digitMatrix[digit][i]);
  }
  digitalWrite (pinDP, decimalPoint);
}

void setup() {
  for (int index = 0; index < segSize; index++){
    pinMode (segments[index], OUTPUT);
  }
  Serial.begin(9600);

  for (int i = 0; i < NUMREADINGS; i++){
    readings[i] = 0;
  }

}

void loop() {
  val = analogRead (antennaPin);
  if (val > 0){ // ignore the readings of 0
    val = constrain (val, 1, senseLimit);
    val = map (val, 1, senseLimit, 0, maxAnalogVal);

    total -= readings[index];
    readings[index] = val;
    total += readings[index];
    index++;

    if (index >= NUMREADINGS){
      index = 0;
    }

    average = total / NUMREADINGS; // averaging the readings
    
    average = map (average, 0, maxAnalogVal, 0, 9);
    if (average == 0){
      dpState = LOW;
    }
    else{
      dpState = HIGH;
    }
  }
    if (millis() - lastUpdateTime >= updateTime){ // update the display number every 0.2s using millis
      displayNumber (average, dpState);
      lastUpdateTime = millis();
      if (average == 0){ // if the number is 0, do not buzz
        noTone(buzzerPin);
      }
      else{
        tone (buzzerPin, average* buzzerTone); // for every level there is a higher pitch
      }
    }
}
