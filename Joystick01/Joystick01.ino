#include <ArduinoJson.h>

#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#define LED_PIN 13
// PIN 12
#define USB_VOLTAGE_PIN A11 
//PIN 9
#define BATTERY_VOLTAGE_PIN A9
long lastCommandTime = 0;

uint8_t myID = 0;



bool wasChargingLastUpdate = false;

void setup() {

  // TODO : if low power, immediately go to sleep

  setupWatchdog();
  checkLowBattery();

  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  pinMode(USB_VOLTAGE_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  updatePowerStatus();

  Serial.begin(57600);
  // uncomment while testing
  while ((!Serial) && (millis() < 3000) && (!isButtonPressed())) sodaq_wdt_safe_delay(1);

  Serial.println("WAKE UP");

    if (isButtonPressed()) {
      updatePowerStatus();
      unsigned long startTime = millis();
      while (isButtonPressed() || ((unsigned long)(millis() - startTime) < 2000)) {
        updatePowerStatus();
        changeColour(getColourForBattery());
        sodaq_wdt_safe_delay(50);
        changeColour(0);
        sodaq_wdt_safe_delay(50);
      }
    }
  setupRadio();

  Serial.println("FIRMWARE v1.0"); 
  Serial.print("ID : "); 
  Serial.println(myID);
}
void checkLowBattery() {
  if (isBatteryLow()) {
    changeColour(255, 0, 0);
    sodaq_wdt_safe_delay(50);
    changeColour(0);
    systemSleep();
  }
}

void loop() {

  // digitalWrite(LED_BUILTIN,digitalRead(USB_VOLTAGE_PIN));

  //getId();
  updatePowerStatus();

  checkMessages();

  // reset watchdog timer
  sodaq_wdt_reset();

  checkLowBattery();

  if (isButtonPressed()) {
    int restoreColour = lastColour;

    unsigned long startTime = millis();
    while (isButtonPressed()) {
      if ((unsigned long)(millis() - startTime) > 400) changeColour(getColourForBattery());
      sodaq_wdt_safe_delay(1);
      if ((unsigned long)(millis() - startTime) > 3000) systemSleep();
    }
    changeColour(restoreColour);
  }


  if (isBatteryCharging()) {
    //Serial.println(getCurrentVoltage()); 
    if ((long)(millis() - lastCommandTime) > 5000) {

      if (isBatteryFull()) {
        changeColour(0x00ff00, 0.01);
      } else {
        changeColour((millis() % 2000 < 10 ? getColourForBattery() : 0), 0.1);
      }
      digitalWrite(LED_BUILTIN, millis() % 500 < 250);


    }
    wasChargingLastUpdate = true;
  } else if (wasChargingLastUpdate) {
    changeColour(0x000000);
    wasChargingLastUpdate = false;
  }

}
