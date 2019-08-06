
const int UP = A4;
const int DOWN = A3; 
const int LEFT = A2; 
const int RIGHT = A5; 
const int FIRE = A1; 

int buttonPins[] = {UP, DOWN, LEFT, RIGHT, FIRE}; 



void setup() {
  // put your setup code here, to run once:
  pinMode(A0, OUTPUT); 
  digitalWrite(A0, LOW); 
  pinMode(LED_BUILTIN, OUTPUT); 

  for(int i = 0; i<5; i++) { 
    pinMode(buttonPins[i], INPUT_PULLUP); 
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  bool ledon = false;   
  for(int i = 0; i<5; i++) { 
    if(!digitalRead(buttonPins[i])) ledon = true; 
  }
  digitalWrite(LED_BUILTIN, ledon); 
}
