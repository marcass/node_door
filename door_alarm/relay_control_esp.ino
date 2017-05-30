//#define debug

// SETUP SERIAL COMM
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete



const int SENSOR = 11;
const int ledpin = 13;
int door_state;
int reed;

volatile int STATE;
volatile unsigned long time_since_trigger;

//trigger states
const int CLOSED = 1;
const int OPEN = 2;

//door states
const int DOOR_OPEN = 0;
const int DOOR_CLOSED = 1;
//const int DOOR_UNKNOWN = 2;

const int cool_down_time = 1;


void setup(){
  //start serial connection
  Serial.begin(115200);
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(ledpin, OUTPUT);

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
  //check_door();
  delay(50);
}

void check_door() {
  //prev_state = door_state;
  open_reed = digitalRead(OPEN);
  closed_reed = digitalRead(CLOSED);
  if ((open_reed == LOW) && (closed_reed == HIGH)) {
    door_state = DOOR_OPEN;
  }else if ((open_reed == HIGH) && (closed_reed == LOW)) {
    door_state = DOOR_CLOSED;
  } else {
    door_state = DOOR_UNKNOWN;
  }
  //publish if changed
  if (door_state != prev_state) {
    switch (door_state) {
      case DOOR_CLOSED:
        Serial.println("m:publish(\"door/state\",\"Closed\",0,1, function(conn) end )");//fix to reflect variable
        break;
      case DOOR_OPEN:
        Serial.println("m:publish(\"door/state\",\"Open\",0,1, function(conn) end )"); //fix to reflect variable
        break;
      case DOOR_UNKNOWN:
        Serial.println("m:publish(\"door/state\",\"Unknown\",0,1, function(conn) end )");//fix to reflect variable
        break;
    }
    prev_state = door_state;
  }
}

void waiting(){
  if (stringComplete) {
    #ifdef debug
      Serial.println("stuff from serial port");
    #endif
    if (inputString.startsWith("Kick door")) {
      #ifdef debug
        Serial.println("door kicked");
      #endif
      inputString = "";
      stringComplete = false;
      STATE = TRIGGER;
      time_since_trigger = millis();
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
  digitalWrite(relaypin, LOW);
  digitalWrite(ledpin, HIGH);
  delay(relay_high_time);
  digitalWrite(relaypin, HIGH);
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

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();  // add it to the inputString:
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
