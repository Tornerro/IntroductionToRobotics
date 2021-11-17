#include <EEPROM.h>

const int dataPin = 12;   //DS
const int latchPin = 11;  // STCP
const int clockPin = 10;  // SHCP

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

int digitArray[] = {
//A B C D E F G DP 
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
};


int displayDigits[] ={
  segD1, segD2, segD3, segD4
};

const int displayCount = 4;

const int xPin = A0;
const int yPin = A1;
const int swPin = 3;

int xValue = 0;
int yValue = 0;
bool swState = 0;

int minThreshold = 300;
int maxThreshold = 750;

bool joyMoved = false;

int digits[4];
bool dpState = 0;
long lastDpChange = 0;
int dpChangeTime = 300;

bool state = 0; // 0 for the line, 1 for the digit

int currentDigit = 0;

long lastPush = 0;
int switchPushDelay = 300;


void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  pinMode (xPin, INPUT);
  pinMode (yPin, INPUT);
  pinMode (swPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(swPin), switchState, FALLING); // interrupt when the swPin goes from HIGH to LOW

  for (int i = 0; i < displayCount; i++){
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  for (int i = 0; i < displayCount; i++){
    digits[i] = EEPROM.read(i);  // i get the past values from eeprom
  }
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  xValue = analogRead (xPin);
  yValue = analogRead (yPin);
  
  if(state == 0) { // if I'm in state 0 i must move through each digit and the dp must blink
    if(yValue > maxThreshold && joyMoved == false){
      currentDigit++;
      joyMoved = 1;
    }
  
    if(yValue < minThreshold && joyMoved == false){
      currentDigit--;
      joyMoved = 1;
    }
  
    if(yValue > minThreshold && yValue < maxThreshold){
      joyMoved = 0;
    }

    if(currentDigit < 0) {
      currentDigit = 3;
    }

    if(currentDigit > 3) {
      currentDigit = 0;
    }
  
    long currentTime = millis();
    if(currentTime - lastDpChange > dpChangeTime) { // i change the dp state every 300ms
      dpState = !dpState;
      lastDpChange = currentTime;  
    }
  }
  if(state == 1) { // if i am in state 1 the dp will have the value HIGH all the time and i must increment or decrement the current digit
      if(xValue > maxThreshold && joyMoved == false){
        digits[currentDigit]--;
        joyMoved = 1;
      }
    
      if(xValue < minThreshold && joyMoved == false){
        digits[currentDigit]++;
        joyMoved = 1;
      }
    
      if(xValue > minThreshold && xValue < maxThreshold){
        joyMoved = 0;
      }
  
      if(digits[currentDigit] < 0) {
        digits[currentDigit] = 9;
      }
  
      if(digits[currentDigit] > 9) {
        digits[currentDigit] = 0;
      }
  }
  EEPROM.write(currentDigit, digits[currentDigit]);
  writeNumber();
}

void writeNumber() {
  int currentNumber = 0;
  for (int i = 3; i >= 0; i--) { // i create the number that will be displayed
    currentNumber *= 10;
    currentNumber += digits[i];
  }
  int lastDigit;
  int currentDisplay = 0;

  while (currentDisplay < displayCount){  // i check for the currentDisplay so i can show numbers like 0123, 0023 etc.
    lastDigit = currentNumber % 10;
    showDigit(currentDisplay);
    if(currentDigit != currentDisplay) { // if the current digit from loop is not the current display i am writing the DP wil stay off
      writeReg(digitArray[lastDigit]);
    }
    else {
      if(state == 0) {            // since the last bit is the one that defines the DP value, i can add 1 or 0 to set the DP LOW or HIGH
        writeReg(digitArray[lastDigit]+dpState); // if the current digit from loop is the current display i am writing and the state is 0 the DP will blink, so i use the dpState that changes in loop
      }
      else if(state == 1) {
        writeReg(digitArray[lastDigit]+1); // if the current digit from loop is the current display i am writing and the state is 1 the DP will be ON
    }
    delay(5);
    currentNumber /= 10;
    currentDisplay++;
  }
}

void writeReg(int digit){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void showDigit(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

void switchState() { // the interrupt function
  int currentPushTime = millis();
  if(currentPushTime - lastPush > switchPushDelay) { // if the last push was made at least 300ms ago the state changes, otherwise it stays the same
    state = !state;
    lastPush = millis();
  }
}
