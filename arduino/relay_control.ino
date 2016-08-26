const int relayPin = 5;     // the number of pin to drive relay
const int ledPin =  13;      // the number of the LED pin

// variables will change:
volatile int inputPin = 2;         // variable for reading the pushbutton status

void setup() {
  //Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(relayPin, OUTPUT);
  // Attach an interrupt to the ISR vector (digital interrupt 0 maps to D2)
  attachInterrupt(0,pin_ISR , RISING);
}

void loop() {
  //test to see if interrupt has been called and delay if it has
  if (digitalRead(relayPin) == HIGH){
    //Serial.println("received high in loop, doing stuff");
    digitalWrite(ledPin, LOW);
    delay(1000);
    digitalWrite(relayPin, LOW);
  }
  else {
    //Serial.println("doign nothing");
    //do nothing
  }
}


void pin_ISR() {
  digitalWrite(relayPin, HIGH);
  //Serial.println("Received interrupt");
}

