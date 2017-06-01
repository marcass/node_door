//#define debug

const int SENSOR = 11; //door sensor connected one end here and the other end ground
const int ledpin = 13;
int state;

//door states
const int OPEN = 0;
const int CLOSED = 1;
//const int DOOR_UNKNOWN = 2;

void setup(){
  //start serial connection
  Serial.begin(115200);
  pinMode(SENSOR, INPUT_PULLUP); //wire to pin 11 pulled up internall, when reed closed (door closed) it gets pulled to ground
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, LOW);
  state = CLOSED;
}

void loop(){
  switch (state) {
    case OPEN:
      proc_open();
      break;
    case CLOSED:
      proc_closed();
      break;
  }
  delay(50);
}

void proc_open() {
  if(digitalRead(SENSOR) == LOW) {
    #ifdef debug
      Serial.println("Door Closed");
    #endif
    //send message to broker via ESP8266 module
    Serial.println("m:publish(\"door/state\",\"Closed\",0,1, function(conn) end )");
    state = CLOSED;
  }
}

void proc_closed() {
  if (digitalRead(SENSOR) == HIGH) {
    #ifdef debug
      Serial.println("Door Opened");
    #endif
    //send message to broker via ESP8266 module
    Serial.println("m:publish(\"door/state\",\"Opened\",0,1, function(conn) end )");
    state = OPEN; //to close
  }
}

