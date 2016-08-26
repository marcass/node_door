const int relayPin = 5;     // the number of pin to drive relay
const int ledPin =  13;      // the number of the LED pin

// variables will change:
volatile int inputPin = 2;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(relayPin, INPUT);
  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(inputPin),pin_ISR , RISING);
}

void loop() {
  //test to see if interrupt has been called and delay if it has
  if (digitalRead(relayPin) == HIGH){
    delay(1000);
    digitalWrite(relayPin, LOW);
  }
  else {
    //do nothing
  }
}


void pin_ISR() {
  digitalWrite(relayPin, HIGH);
}

