const int relayPin = 5;     // the number of pin to drive relay
const int ledPin =  13;      // the number of the LED pin
const int inputPin = 2;         // variable for reading the pushbutton status

// variables will change:
//volatile int inputPin = 2;         // variable for reading the pushbutton status
//volatile int start_time;
unsigned long start_time;
const int wait_time_min = 300;    // time [ms] for
const int cut_off_time = 600;  // time [ms] for input pin to be HIGH before deciding to trigger relay 
const int trigger_duration = 200;  // time [ms] that relay is pulled HIGH for.
int breach_min;
int triggered;

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(relayPin, OUTPUT);
  // Attach an interrupt to the ISR vector (digital interrupt 0 maps to D2)
  //attachInterrupt(0,pin_ISR , RISING);
  pinMode(inputPin, INPUT);
  start_time = millis();
  breach_min = 0;
}

void loop() {
  //test to see if input pin HIGH and breaches wait time
  if ((digitalRead(inputPin) == HIGH) && (triggered == 0)) {
    triggered = 1;
    start_time = millis();
    }
    
  
  if (triggered == 1) {
    if ((millis() - start_time) > wait_time_min) {
      Serial.println("threshold breached");
      breach_min = 1;
    }
    else {
      Serial.println("waiting some more for breach");
      //breach_min = 1;
    }
  }
  else {
    //do nothing
  }
  if (breach_min == 1) {
    if (digitalRead(inputPin) == LOW) {
      if ((millis() - start_time) < cut_off_time) {
        Serial.println("not on too long so triggering");
        breach_min = 0;
        triggered = 0;
        //trigger relay
        digitalWrite(ledPin, HIGH);
        digitalWrite(relayPin, HIGH);
        delay(trigger_duration);
        digitalWrite(ledPin, LOW);
        digitalWrite(relayPin, LOW);
      }
      else if (digitalRead(inputPin) == HIGH) {
        Serial.println("Waiting for pin to come down");
      }
      else if ((millis() - start_time) > cut_off_time){
        Serial.println("ON too long");
          delay(1000);
          //reset everything and wait for things to come into line again
          start_time = millis();
          triggered = 0;
          breach_min = 0;
          Serial.print("breach_min = ");
          Serial.println(breach_min);
          Serial.print("triggered = ");
          Serial.println(triggered);
          Serial.println(digitalRead(inputPin));

      }
      else {
        Serial.println("Doing nothing");
      }
    }
    else {
      //do nothing while looping some more
    }
  }
}
