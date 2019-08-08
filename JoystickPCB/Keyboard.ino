#include "Keyboard.h"

const int UP_KEY = KEY_UP_ARROW;
const int DOWN_KEY = KEY_DOWN_ARROW;
const int LEFT_KEY = KEY_LEFT_ARROW;
const int RIGHT_KEY = KEY_RIGHT_ARROW;
const int FIRE_KEY = KEY_RETURN;

int buttonKeys[] = {UP_KEY, DOWN_KEY, LEFT_KEY, RIGHT_KEY, FIRE_KEY};
uint8_t lastKeyboardFlags = 0;

void setupKeyboard() { 
  Keyboard.begin(); 
}


void updateKeys(int newflags) { 
  digitalWrite(LED_BUILTIN, HIGH); 

  long keystarttime = millis(); 
  for (int i = 0; i < 5; i++ ) {
    bool oldstate = (lastKeyboardFlags >> i) & 1;
    bool newstate = (newflags >> i) & 1;
//    Serial.print(i);
//    Serial.print(" ") ;
//    Serial.print(oldstate);
//    Serial.print(" ");
//    Serial.print(newstate);
//    Serial.print(" ");
    
    if (oldstate != newstate) {
      if (newstate) {
        //Serial.print(i); Serial.println(" on");
        if(keySendActive) Keyboard.press(buttonKeys[i]);
      } else {
       // Serial.print(i); Serial.println(" off");
        if(keySendActive) Keyboard.release(buttonKeys[i]);
      }
    } 


  }
//  digitalWrite(LED_BUILTIN, LOW); 
//  Serial.println(millis()-keystarttime); 
  if(millis()-keystarttime>100) keySendActive = false;
  if(keySendActive) lastKeyboardFlags = newflags; 
  
}
