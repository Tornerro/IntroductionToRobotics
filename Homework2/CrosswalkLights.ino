const int pushButton = 13;
const int pedestriansRedPin = 12;
const int pedestriansGreenPin = 11;
const int carsGreenPin = 10;
const int carsYellowPin = 9;
const int carsRedPin = 8;
const int buzzerPin = 6;

const int state1Wait = 10000;
const int state2Wait = 3000;
const int state3Wait = 10000;
const int state3BuzzInterval = 1000;
const int state4Wait = 4000;
const int state4BuzzInterval = 500;
const int buzzerTone = 500;

int systemState = 1;
int state1ButtonPress = 0;
int state1Timer = 0;
int state2EnterTime = 0;
int state2Timer = 0;
int state3EnterTime = 0;
int state3Timer = 0;
int state4EnterTime = 0;
int state4Timer = 0;
int lastBuzzerChange = 0;

bool buttonState = 0;
bool buttonWasPressed = 0;
bool enteredState2 = 0;
bool enteredState3 = 0;
bool enteredState4 = 0;
bool isBuzzerOn = 0;

void setup() {
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(pedestriansRedPin, OUTPUT);
  pinMode(pedestriansGreenPin, OUTPUT);
  pinMode(carsGreenPin, OUTPUT);
  pinMode(carsYellowPin, OUTPUT);
  pinMode(carsRedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  switch (systemState){
    case 1: 
      state1(); 
      break;
    case 2: 
      state2(); 
      break;
    case 3:
      state3();
      break;
    case 4:
      state4();
      break;
    default:
      break;
  }
}



void state1(){
  digitalWrite (carsGreenPin, HIGH);
  digitalWrite (pedestriansRedPin, HIGH);
  buttonState = digitalRead(pushButton);
  if (!buttonState && !buttonWasPressed){
    state1ButtonPress = millis();
    buttonWasPressed = 1;
  }
  if (buttonWasPressed){
    state1Timer = millis();
    if (state1Timer - state1ButtonPress >= state1Wait){
      systemState = 2;
      digitalWrite (carsGreenPin, LOW);
      buttonWasPressed = 0;
    }
  }
}

void state2(){
  digitalWrite (carsYellowPin, HIGH);
  if (!enteredState2){
    enteredState2 = 1;
    state2EnterTime = millis();
  }
  state2Timer = millis();
  if (state2Timer - state2EnterTime >= state2Wait){
    systemState = 3;
    digitalWrite (carsYellowPin, LOW);
    digitalWrite (pedestriansRedPin, LOW);
    enteredState2 = 0;
  }
}

void state3(){
  digitalWrite (carsRedPin, HIGH);
  digitalWrite (pedestriansGreenPin, HIGH);
  if (!enteredState3){
    enteredState3 = 1;
    state3EnterTime = millis();
    lastBuzzerChange = millis();
    isBuzzerOn = 1;
    tone (buzzerPin, buzzerTone);
  }
  state3Timer = millis();

  if (state3Timer - lastBuzzerChange >= state3BuzzInterval){
    if (isBuzzerOn){
      noTone (buzzerPin);
      isBuzzerOn = 0;
    }
    else{
      tone (buzzerPin, buzzerTone);
      isBuzzerOn = 1;
    }
    lastBuzzerChange = millis();
  }

  if (state3Timer - state3EnterTime >= state3Wait){
    if (isBuzzerOn){
      isBuzzerOn = 0;
      noTone (buzzerPin);
    }
    systemState = 4;
    enteredState3 = 0;
  }
}

void state4(){
  if (!enteredState4){
    enteredState4 = 1;
    state4EnterTime = millis();
    lastBuzzerChange = millis();
    isBuzzerOn = 1;
    tone (buzzerPin, buzzerTone);
  }
  state4Timer = millis();

   if (state4Timer - lastBuzzerChange >= state4BuzzInterval){
    if (isBuzzerOn){
      noTone (buzzerPin);
      isBuzzerOn = 0;
      digitalWrite (pedestriansGreenPin, LOW);
    }
    else{
      tone (buzzerPin, buzzerTone);
      isBuzzerOn = 1;
      digitalWrite (pedestriansGreenPin, HIGH);
    }
    lastBuzzerChange = millis();
  }

  if (state4Timer - state4EnterTime >= state4Wait){
    if (isBuzzerOn){
      isBuzzerOn = 0;
      noTone (buzzerPin);
    }
    digitalWrite (pedestriansGreenPin, LOW);
    digitalWrite (carsRedPin, LOW);
    systemState = 1;
    enteredState4 = 0;
  }
}
