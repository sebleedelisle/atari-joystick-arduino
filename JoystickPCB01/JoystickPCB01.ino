

#include <Adafruit_NeoPixel.h>
//#include <Adafruit_SleepyDog.h>

//#define USE_BLUETOOTH

const int UP = 5;
const int DOWN = 6;
const int LEFT = 10;
const int RIGHT = 3;
const int FIRE = 11;

const int BRIGHTNESS = 255;

const int BATTERY_CHARGE_PIN = 1;
bool keySendActive = true; 
const int neoPixelPin = 2;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, neoPixelPin, NEO_GRB + NEO_KHZ800);

const int RECEIVER_ADDRESS = 0;
long lastMessageTime = 0;

int buttonPins[] = {UP, DOWN, LEFT, RIGHT, FIRE};

uint8_t myID = 1;
long lastCommandTime = 0;
long lastWatchdogReset = 0;
int watchdogMaxTime = 0;
long lastErrorTime = -1;

uint8_t statusFlags = 0;
uint64_t lastColour = 0;

void setup() {
  setupRadio();
  Serial.begin(9600);

  setupKeyboard();

  // int watchdogMaxTime = Watchdog.enable(10000);
  //Serial.print("Watchdog ms : ");
  //Serial.println(countdownMS);
  //Watchdog.disable();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BATTERY_CHARGE_PIN, INPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();



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
    if (!sendStatus(statusFlags)) lastErrorTime = millis();

    updateKeys(oldflags, statusFlags);
    // debounce
    while (millis() - timer < 1) delay(1);
    lastMessageTime = millis();
  }

  uint64_t colour = getColourForBattery();
  //  uint32_t red = colour >> 16;
  //  uint32_t green = (colour >> 8) & 0xff;
  //Serial.println(colour, HEX);
  bool ledOn = false;

  if ((lastErrorTime < 0) || (keySendActive)) {
    if ((millis() < 2000) && (millis() % 200 < 100)) ledOn = true;
    else if ((millis() - lastMessageTime < 300) && (millis() % 100 < 50)) ledOn = true;

    digitalWrite(LED_BUILTIN, (millis() % 800 < 100));
    colour = ledOn ? colour : 0;
  } else {
    //last message failed!
    int shortdelay = 25;
    int longdelay = 50;

    int flashcount = 2;

    int patternlength = ((shortdelay * flashcount * 2) + longdelay) * 2;
    int progress = millis() % patternlength;
    uint64_t flashcolour =  (progress < patternlength >> 1) ? strip.Color(255, 0, 0, 0) : strip.Color(0, 0, 255, 0);
    int halfpatternprogress = progress % (patternlength >> 1);
    colour =  ((halfpatternprogress < shortdelay * flashcount * 2) && ((halfpatternprogress % (shortdelay * 2)) < shortdelay)) ? flashcolour : 0;


    if (millis() - lastErrorTime > 1000) lastErrorTime = -1;
  }
  if (!digitalRead(BATTERY_CHARGE_PIN)) {
    uint64_t chargeColour = getColourForBattery(); 
    int r = chargeColour>>16; 
    int g = (chargeColour>>8) & 0xff;
    int b = chargeColour & 0xff; 
    float brightness = ((sin((float)millis()/100.0f) + 1.0f )*0.5f);
    brightness = (brightness*0.05) + 0.05f;  
    colour |= strip.Color(r*brightness, g*brightness, b*brightness);
  }
  //if(lastColour!=colour) {
  strip.setPixelColor(0, colour);
  strip.show();
  lastColour = colour;
  //}
  //  if (millis() - lastWatchdogReset > (  2000)) {
  //    Watchdog.reset();
  //    lastWatchdogReset = millis();
  //  }
}



uint8_t getId() {
  return 1;
};
