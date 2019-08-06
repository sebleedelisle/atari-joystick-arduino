
#include <Adafruit_SleepyDog.h>

const int UP = A4;
const int DOWN = A3;
const int LEFT = A2;
const int RIGHT = A5;
const int FIRE = A1;

const int redLedPin = 10;
const int greenLedPin = 11;

const int RECEIVER_ADDRESS = 0;
long lastMessageTime = 0;

int buttonPins[] = {UP, DOWN, LEFT, RIGHT, FIRE};

uint8_t myID = 1;
long lastCommandTime = 0;
long lastWatchdogReset = 0;
int watchdogMaxTime = 0;
long lastErrorTime = -1;

uint8_t statusFlags = 0;

void setup() {
  setupRadio();
  Serial.begin(9600);
  setupKeyboard();

  int watchdogMaxTime = Watchdog.enable(10000);
  //Serial.print("Watchdog ms : ");
  //Serial.println(countdownMS);
  //Watchdog.disable();

  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(LED_BUILTIN, OUTPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);



}

void loop() {

  bool ledon = false;
  uint8_t oldflags = statusFlags;
  statusFlags = 0;
  for (int i = 0; i < 5; i++) {
    if (!digitalRead(buttonPins[i])) {
      statusFlags |= (1 << i);
      ledon = true;
    }
  }
  //digitalWrite(LED_BUILTIN, ledon);
  if ((oldflags != statusFlags) || (millis() - lastMessageTime > 3000)) {
    long timer = millis();
    if(!sendStatus(statusFlags)) lastErrorTime = millis();

    updateKeys(oldflags, statusFlags);
    // debounce
    while (millis() - timer < 50) delay(1);
    lastMessageTime = millis();
  }

  uint32_t colour = getColourForBattery();
  uint32_t red = colour >> 16;
  uint32_t green = (colour >> 8) & 0xff;
  //Serial.println(colour, HEX); 
  bool ledOn = false;

  if (lastErrorTime <0) {
    if ((millis() < 2000) && (millis() % 200 < 100)) ledOn = true;
    else if ((millis() - lastMessageTime < 300) && ((millis() % 100 < 50)))ledOn = true;
    //Serial.println(millis() - lastMessageTime);
    //ledOn = Serial;
    analogWrite(redLedPin, ledOn ? red : 0);
    analogWrite(greenLedPin, ledOn ? green : 0);
    //digitalWrite(LED_BUILTIN, (millis() % 800 < 100));
  } else {
    //last message failed!
    int modmillis = millis()%400; 
    red = ((modmillis < 100)||((modmillis > 200) && (modmillis<300))) ? 255 : 0;
    green = ((modmillis > 200) && (modmillis<300)) ? 255 : 0;
    analogWrite(redLedPin, red);
    analogWrite(greenLedPin, green);
    if(millis()-lastErrorTime>1000) lastErrorTime = -1; 
  }


  if (millis() - lastWatchdogReset > (  2000)) {
    Watchdog.reset();
    lastWatchdogReset = millis();
  }
}



uint8_t getId() {
  return 1;
};
