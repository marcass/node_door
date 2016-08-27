const int inputpin = 2;
const int relaypin = 5;
const int ledpin = 13;

volatile int STATE;
volatile unsigned long time_since_trigger;

const int WAITING = 1;
const int PRELIMINARY_TRIGGER = 2;
const int TRIGGERED = 3;
const int TOO_LONG_TRIGGER = 4;

const int min_triggertime = 300;
const int max_triggertime = 1000;
const int relay_high_time = 100;
const int cool_down_time = 1;


void setup(){
  //start serial connection
  Serial.begin(115200);
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(inputpin, INPUT);
  pinMode(relaypin, OUTPUT); 
  pinMode(ledpin, OUTPUT);
  digitalWrite(relaypin, LOW);
  digitalWrite(ledpin, LOW);
  STATE = WAITING;
}

void loop(){
  if (STATE == WAITING) {waiting();}
  else if (STATE == PRELIMINARY_TRIGGER) {pre_trigger();}
  else if (STATE == TRIGGERED) {triggered();}
  else if (STATE == TOO_LONG_TRIGGER) {too_long();}
}


void waiting(){
  if (digitalRead(inputpin) == HIGH) {
    STATE = PRELIMINARY_TRIGGER;
    Serial.println("going to PRELIMINARY_TRIGGER state");
    time_since_trigger = millis();
    Serial.println(time_since_trigger);
  }
}

void pre_trigger(){
  int dt = millis() - time_since_trigger;
  if ( digitalRead(inputpin) == HIGH) {  
    if (dt > max_triggertime) {
      STATE = TOO_LONG_TRIGGER;
      Serial.println(time_since_trigger);
      Serial.println("going to TOO_LONG_TRIGGER state");
    }
  } else {
    // input pin is LOW
    if  (dt >  min_triggertime){
      STATE = TRIGGERED;
      Serial.println("going to TRIGGERED state");
    } else {
      STATE = WAITING;
      Serial.println("going to WAITING state");
    }
  }
}

void triggered(){
  digitalWrite(relaypin, HIGH);
  digitalWrite(ledpin, HIGH);
  delay(relay_high_time);
  digitalWrite(relaypin, LOW);
  digitalWrite(ledpin, LOW);
  delay(cool_down_time);
  STATE = WAITING;
  Serial.println("going to WAITING state");
}

void too_long(){
  if (digitalRead(inputpin) == LOW) {
    delay(cool_down_time);
    STATE = WAITING;
    time_since_trigger = millis();
    Serial.println(time_since_trigger);
    Serial.println("going to WAITING state");
  }
}
