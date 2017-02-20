#define debug

// SETUP SERIAL COMM
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete



const int CLOSED = 12;
const int OPEN = 11;
const int relaypin = 5;
const int ledpin = 13;
int door_state;
int open_reed;
int closed_reed;
int prev_state = 2;

volatile int STATE;
volatile unsigned long time_since_trigger;

//trigger states
const int WAITING = 1;
const int PRELIMINARY_TRIGGER = 2;
const int TRIGGER = 3;
const int TOO_LONG_TRIGGER = 4;

//door states
const int DOOR_OPEN = 0;
const int DOOR_CLOSED = 1;
const int DOOR_UNKNOWN = 2;

const int min_triggertime = 300;
const int max_triggertime = 1000;
const int relay_high_time = 200;
const int cool_down_time = 1;


void setup(){
  //start serial connection
  Serial.begin(9600);
  inputString.reserve(200); // reserve mem for received message on serial port
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(OPEN, INPUT_PULLUP);
  pinMode(CLOSED, INPUT_PULLUP);
  pinMode(relaypin, OUTPUT); 
  pinMode(ledpin, OUTPUT);
  digitalWrite(relaypin, LOW);
  digitalWrite(ledpin, LOW);
  STATE = WAITING;
}

void loop(){
  switch (STATE) {
    case WAITING:
      waiting();
      break;
    case TRIGGER:
      triggered();
      break;
  }
  if (stringComplete){
    inputString = "";
    stringComplete = false;
  }
  //read state of door
  if (open_reed != digitalRead(OPEN)){
    //something has changed so set prev state and check
    prev_state = door_state;
    check_door();
  }
  if (closed_reed != digitalRead(CLOSED)) {
    //something has changed
    prev_state = door_state;
    check_door();
  }
  delay(50);
}

void check_door() {
  open_reed = digitalRead(OPEN);
  closed_reed = digitalRead(CLOSED);
  if ((open_reed == LOW) && (closed_reed == HIGH)) {
    door_state = DOOR_OPEN;
  } else if ((open_reed == HIGH) && (closed_reed == LOW)) {
    door_state = DOOR_CLOSED;
  } else {
    door_state = DOOR_UNKNOWN;
  }
  //publish if changed
  if (door_state != prev_state) {
    switch (door_state) {
      case DOOR_CLOSED:
        Serial.println("m:publish(\"door/state\",Closed,0,0, function(conn) end )");//fix to reflect variable
        break;
      case DOOR_OPEN:
        Serial.println("m:publish(\"door/state\",Open,0,0, function(conn) end )"); //fix to reflect variable
        break;
      case DOOR_UNKNOWN:
        Serial.println("m:publish(\"door/state\",Unknown,0,0, function(conn) end )");//fix to reflect variable
        break;
    }
  }
}

void waiting(){
  if (stringComplete) {
    if (inputString.startsWith("Kick door")) {
      inputString = "";
      stringComplete = false;
      STATE = TRIGGER;
      time_since_trigger = millis();
      #ifdef debug
        Serial.println("going to PRELIMINARY_TRIGGER state");
        Serial.println(time_since_trigger);
      #endif
    }
  }
}

//void pre_trigger(){
//  int dt = millis() - time_since_trigger;
//  if ( digitalRead(relaypin) == HIGH) {  
//    if (dt > max_triggertime) {
//      STATE = TOO_LONG_TRIGGER;
//      #ifdef debug
//        Serial.println(time_since_trigger);
//        Serial.println("going to TOO_LONG_TRIGGER state");
//      #endif
//    }
//  } else {
//    // input pin is LOW
//    if  (dt >  min_triggertime){
//      STATE = TRIGGERED;
//      #ifdef debug
//        Serial.println("going to TRIGGERED state");
//      #endif
//    } else {
//      STATE = WAITING;
//      #ifdef debug
//        Serial.println("going to WAITING state");
//      #endif
//    }
//  }
//}

void triggered(){
  digitalWrite(relaypin, HIGH);
  digitalWrite(ledpin, HIGH);
  delay(relay_high_time);
  digitalWrite(relaypin, LOW);
  digitalWrite(ledpin, LOW);
  delay(cool_down_time);
  STATE = WAITING;
  #ifdef debug
    Serial.println("going to WAITING state");
  #endif
}

//void too_long(){
//  if (digitalRead(relaypin) == LOW) {
//    delay(cool_down_time);
//    STATE = WAITING;
//    time_since_trigger = millis();
//    #ifdef debug
//      Serial.println(time_since_trigger);
//      Serial.println("going to WAITING state");
//    #endif
//  }
//}
