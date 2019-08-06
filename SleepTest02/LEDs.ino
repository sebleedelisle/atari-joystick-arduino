

const int greenPin = A2;    // LED connected to digital pin 9
const int redPin = A1;     // LED connected to digital pin 10
const int bluePin = A0;    // LED connected to digital pin 11

bool isButtonPressed() { 

  bool lighton = digitalRead(SLEEP_BUTTON_AND_LED); 
  pinMode(SLEEP_BUTTON_AND_LED, INPUT); 
  digitalWrite(SLEEP_BUTTON_AND_LED, HIGH); 

  delay(1);
  bool state = !digitalRead(SLEEP_BUTTON_AND_LED); 
  Serial.print("BUTTON :"); Serial.println(digitalRead(SLEEP_BUTTON_AND_LED));
  pinMode(SLEEP_BUTTON_AND_LED, OUTPUT);
  digitalWrite(SLEEP_BUTTON_AND_LED, lighton);  
  
  return state;  
  
}

void setupLEDs() {
  pinMode(SLEEP_BUTTON_AND_LED, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  changeColour(0,0,0); 
}

void startupFlashLEDs() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(SLEEP_BUTTON_AND_LED, i % 2);

    for (int j = 50; j >= 0; j-=2) {
      changeColour(0, j/2, 0);
      sodaq_wdt_safe_delay(1);
    }
  }

}
void changeColour(int colour) {
  changeColour((colour >> 16), (colour >> 8 & 0xff), (colour & 0xff));
}



void changeColour(int r, int g, int b) {
  float bright = 1;
  analogWrite(redPin, r * bright);
  analogWrite(greenPin, g * bright);
  analogWrite(bluePin, b * bright);
}

