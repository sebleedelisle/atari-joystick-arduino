

#include "Encoder.h"
#include "headers.h"
#include <HSBColor.h>

#include <Adafruit_NeoPixel.h>


#include "Receiver.h"

const int inputPin = A0;
const int outputPreviewStripPin = 12;
const int receiverStrengthStripPin = 10;
const int buttonPin = 5;

const int checkIntervalMils = 60000;
const int numShoes = 6;
Receiver shoes[numShoes];
int shoeIDs[] = {13,9,10,11,12,14};

unsigned long effectStartTime = 0;
int currentEffect = 0;

int lightOn = 0;

bool buttonPressed = false;

float smoothedRead = 0;
int hue = 0;
int lastHueSent = 0;

Adafruit_NeoPixel outputStrip = Adafruit_NeoPixel(8, outputPreviewStripPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel statusStrip = Adafruit_NeoPixel(16, receiverStrengthStripPin, NEO_GRB + NEO_KHZ800);

int rgb[3];

int currentEffectNum = 0;
int numEffects = 3;

unsigned long counter = 0;

Encoder encoder;
unsigned long lastRadioCheck = -checkIntervalMils;
bool encoderChanged = true; 

void setup() {

  outputStrip.begin();
  outputStrip.show(); // Initialize all pixels to 'off'
  statusStrip.begin();
  statusStrip.show(); // Initialize all pixels to 'off'

  Serial.begin(9600);
  //while (!Serial) { delay(1); }

  pinMode(inputPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  setupRadio();

  for (int i = 0; i < numShoes; i++) {
    shoes[i].id = shoeIDs[i];
  }

}
long oldPosition  = -999;

void loop() {

  checkSerial();

  switch (currentEffect) {
    case 0 :
      if (encoderChanged || ((millis() - lastRadioCheck) > checkIntervalMils)) {
        checkShoeStatuses();
        lastRadioCheck = millis();
      }
      break;
    case 1 :
      runChaseEffect();
      break;
    case 2 :
      runRainbowEffect();
      break;
    case 3 :
      runRainbowFadeEffect();
      break;
    case 4 :
    case 5 :
    case 6 :
    case 7 :
    case 8 :
    case 9 :
    case 10 :
      runColourEffect(currentEffect - 4);
      break;

  }

  counter++;
  //delay(10);
  encoderChanged = checkInputs(); // encoder.tick();

}

void checkSerial() {

  while (Serial.available() > 0) {
    int effectnum = Serial.parseInt();

    if (Serial.read() == '\n') {
      // do something with the int
      if (currentEffect != effectnum) {
        currentEffect = effectnum;
        effectStartTime = millis();
      }
      if (currentEffect == 0) {

        for (int i = 0; i < numShoes; i++) {
          addColourChangeMessageToBuffer(shoeIDs[i], 0, 0, 0);
        }
        sendMessages();
      }
    }
  }
}

void checkShoeStatuses() {

  for (int i = 0; i < numShoes; i++) {
    Receiver& shoe = shoes[i];
    checkReceiverStatus(shoe);
    int r, g, b;
    r = g = b = 0;

    if (shoe.connected) g = 0xff;
    else r = 0xff;

    statusStrip.setPixelColor(7 - i, r >> 5, g >> 5, b >> 5);
    statusStrip.setPixelColor(15 - i,  getColourForBatteryLevel(shoe.batteryLevel));
    // Serial.println(shoe.batteryLevel);

    checkInputs();
  }
  // second attempt for shoes that aren't happening
  for (int i = 0; i < numShoes; i++) {
    Receiver& shoe = shoes[i];
    if (!shoe.connected) checkReceiverStatus(shoe);
    int r, g, b;
    r = g = b = 0;
    if (shoe.connected) {
      g = 0xff;
      statusStrip.setPixelColor(15 - i,  getColourForBatteryLevel(shoe.batteryLevel));
    } else {
      r = 0xff;
      statusStrip.setPixelColor(15 - i,  0);
    }

    statusStrip.setPixelColor(7 - i, r >> 5, g >> 5, b >> 5);
    checkInputs();

  }
  statusStrip.show();
}

int getShoeIndexByID(int id) {

  for (int i = 0; i < numShoes; i++) {
    if (shoeIDs[i] == id) return i;
  }
  return -1;
}

bool checkInputs() {
  encoder.tick();
  long newPosition = encoder.getPos();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
    return true; 
  } else {
    return false; 
  }
  
  
}

int getColourForBatteryLevel(int level) {
  //    Serial.print("batt : ");
  //    Serial.println(level);
  int returncolour;
  if (level >= 420) returncolour =  0x00ffff;
  else if (level > 410) returncolour =  0x00ff00;
  else if (level > 360) returncolour =  0xffff00;
  else returncolour =  0xff0000;

  int r = returncolour >> 16;
  int g = returncolour >> 8 & 0xff;
  int b = returncolour & 0xff;
  r >>= 5;
  g >>= 5;
  b >>= 5;
  return r << 16 | g << 8 | b;

}



void runColourEffect(int effect) {
  int colour;
  Serial.println(effect); 
  switch (effect) {
    case 0 :
      colour = 0xff0000;
      break;
    case 1 :
      colour = 0x00ff00;
      break;
    case 2 :
      colour = 0x0000ff;
      break;
    case 3 :
      colour = 0xffff00;
      break;
    case 4 :
      colour = 0x00ffff;
      break;
    case 5 :
      colour = 0xff00ff;
      break;
    case 6 :
      colour = 0xffffff;
      break;
  }

  for (int i = 0; i < numShoes; i++) {

    addColourChangeMessageToBuffer(shoeIDs[i], colour);

  }

  sendMessages();

}
