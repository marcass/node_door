const int relayPin = 5;     // the number of pin to drive relay
const int ledPin =  13;      // the number of the LED pin
const int inputPin = 2;         // variable for reading the pushbutton status

// variables will change:
//volatile int inputPin = 2;         // variable for reading the pushbutton status
//volatile int start_time;
int start_time;
const int wait_time_min = 300;    // time [ms] for
const int wait_time_max = 400;    // time [ms] for
const int cut_off_time = 750;  // time [ms] for input pin to be HIGH before deciding to trigger relay 
const int trigger_duration = 100;  // time [ms] that relay is pulled HIGH for.
int started;

void setup() {
  //Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(relayPin, OUTPUT);
  // Attach an interrupt to the ISR vector (digital interrupt 0 maps to D2)
  //attachInterrupt(0,pin_ISR , RISING);
  pinMode(inputPin, INPUT);
  start_time = millis();
  started = 0;
}

void loop() {
  //test to see if interrupt has been called and delay if it has
  if ((millis() - start_time)> cut_off_time){
    // do nothing
    if started {
      trigger();
      started = 0;
    } else {
      start_time = millis();
    }
  }
  
  if ((millis() - start_time) > wait_time_min) && ((millis() - start_time) < wait_time_max) {  
    if (digitalRead(inputPin) == HIGH){
      started = 1;
    }
  
//      digitalWrite(ledPin, HIGH);
//      digitalWrite(relayPin, HIGH);
//      delay(trigger_duration);
//      digitalWrite(ledPin, LOW);
//      digitalWrite(relayPin, LOW); 
//      start_time = millis();
     //   started = 0;
     // }
    }
    //Serial.println("received high in loop, doing stuff");
  }
  else {
    
    //start_time = millis();
    //started = 0;
    //Serial.println("doign nothing");
    //do nothing
  }
}


//void pin_ISR() {
//  digitalWrite(relayPin, HIGH);
//  //Serial.println("Received interrupt");
//}

