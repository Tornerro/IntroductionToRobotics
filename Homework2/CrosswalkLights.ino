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
const int state4Wait = 5000;
const int state4BuzzInterval = 500;
const int buzzerTone = 500;

int systemState = 1;
int state1ButtonPress = 0;
int stateTimer = 0;
int stateEnterTime = 0;
int lastBuzzerChange = 0;

unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

bool buttonState = 1;
bool reading = 1;
bool lastReading = 1;
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
  reading = digitalRead(pushButton);

  //check if the button was pressed. if it was pressed, i update buttonWasPressed and start the button timer
  if (!buttonWasPressed){
    if (reading != lastReading){
      lastDebounceTime = millis();
    }
  
    if (millis() - lastDebounceTime > debounceDelay){
      if(reading != buttonState){
        buttonState = reading;
        if (buttonState == LOW){
          state1ButtonPress = millis();
          buttonWasPressed = 1;
        }
      }
    }
    lastReading = reading;
  }

  
  if (buttonWasPressed){
    stateTimer = millis();
    //when the state wait time passes i reset buttonWasPressed and change the state and stop the state led
    if (stateTimer - state1ButtonPress >= state1Wait){ 
      systemState = 2;
      digitalWrite (carsGreenPin, LOW);
      buttonWasPressed = 0;
    }
  }
}

void state2(){
  digitalWrite (carsYellowPin, HIGH);
  //when i enter the state i start the timer
  if (!enteredState2){
    enteredState2 = 1;
    stateEnterTime = millis();
  }
  stateTimer = millis();
  //when the state wait period ends i change the state value and turn off the state leds
  if (stateTimer - stateEnterTime >= state2Wait){
    systemState = 3;
    digitalWrite (carsYellowPin, LOW);
    digitalWrite (pedestriansRedPin, LOW);
    enteredState2 = 0;
  }
}

void state3(){
  digitalWrite (carsRedPin, HIGH);
  digitalWrite (pedestriansGreenPin, HIGH);
  //when i enter the state i start the timer and turn on the buzzer
  if (!enteredState3){
    enteredState3 = 1;
    stateEnterTime = millis();
    lastBuzzerChange = millis();
    isBuzzerOn = 1;
    tone (buzzerPin, buzzerTone);
  }
  stateTimer = millis();

  // after the buzz interval time passes i switch the buzzer's state
  if (stateTimer - lastBuzzerChange >= state3BuzzInterval){
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
  
  // after the state wait time i turn off the buzzer again to be sure it won't keep on buzzing, then change the state
  if (stateTimer - stateEnterTime >= state3Wait){
    if (isBuzzerOn){
      isBuzzerOn = 0;
      noTone (buzzerPin);
    }
    systemState = 4;
    enteredState3 = 0;
  }
}

void state4(){
  // same thing as state 3, but the buzz switches at a faster frequency and now the green pedestrian led switches toghetgher with the buzzer
  if (!enteredState4){
    enteredState4 = 1;
    stateEnterTime = millis();
    lastBuzzerChange = millis();
    isBuzzerOn = 1;
    tone (buzzerPin, buzzerTone);
  }
  stateTimer = millis();

   if (stateTimer - lastBuzzerChange >= state4BuzzInterval){
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

  if (stateTimer - stateEnterTime >= state4Wait){
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
