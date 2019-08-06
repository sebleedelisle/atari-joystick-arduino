

#include "Keyboard.h"

const int MY_ADDRESS = 0;
const int UP = KEY_UP_ARROW;
const int DOWN = KEY_DOWN_ARROW;
const int LEFT = KEY_LEFT_ARROW;
const int RIGHT = KEY_RIGHT_ARROW;
const int FIRE = KEY_RETURN;

int buttonKeys[] = {UP, DOWN, LEFT, RIGHT, FIRE};

uint8_t myID = 1;
long lastCommandTime = 0;

uint8_t statusFlags = 0;

bool keySendActive = false;
long lastMessageTime = 0;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, keySendActive);



  // A0 and A2 are either side of the pushbutton
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(A1, INPUT);
  digitalWrite(A1, LOW);
  pinMode(A2, INPUT_PULLUP);

  Keyboard.begin();
  while ((!Serial) && (millis() < 1000));
  setupRadio();
}

void setKeys(uint8_t newflags) {
  printBits(newflags);
  printBits(statusFlags);
  if (statusFlags == newflags) return ;

  for (int i = 0; i < 5; i++ ) {
    bool oldstate = (statusFlags >> i) & 1;
    bool newstate = (newflags >> i) & 1;
    Serial.print(i);
    Serial.print(" ") ;
    Serial.print(oldstate);
    Serial.print(" ");
    Serial.print(newstate);
    Serial.print(" ");

    if (oldstate != newstate) {
      if (newstate) {
        Serial.print(i); Serial.println(" on");
        if (keySendActive) Keyboard.press(buttonKeys[i]);
      } else {
        Serial.print(i); Serial.println(" off");
        if (keySendActive) Keyboard.release(buttonKeys[i]);
      }
    } else Serial.println(" ");


  }









  statusFlags = newflags;
}

void loop() {
  if(checkMessages()) lastMessageTime = millis();
  //Serial.println(digitalRead(A2));
  if (!digitalRead(A2)) {
    //  Serial.println(digitalRead(A2));
    keySendActive = !keySendActive;
    if (!keySendActive) Keyboard.releaseAll();
    //debounce
    digitalWrite(LED_BUILTIN, keySendActive);
    delay(1);
    while (!digitalRead(A2)) delay(1);

  }

 
  digitalWrite(LED_BUILTIN, keySendActive^((millis() - lastMessageTime < 300) && ((millis() % 100 < 50))));
 


}


uint8_t getId() {
  return 0;
};
