
#include "Sodaq_wdt.h"

#define SLEEP_BUTTON_AND_LED 13
#define BATTERY_VOLTAGE_PIN A7
#define USB_VOLTAGE_PIN A3




void setup() {
  
  // TODO : if low power, immediately go to sleep
  
  setupLEDs();

  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  pinMode(USB_VOLTAGE_PIN, INPUT);
  pinMode(SLEEP_BUTTON_AND_LED, OUTPUT);
  //digitalWrite(SLEEP_BUTTON, HIGH);

  setupWatchdog();
  
  //pinMode(SLEEP_BUTTON, INPUT);
  //digitalWrite(SLEEP_BUTTON, HIGH); // set pull-up resistor

  startupFlashLEDs();


}

void loop() {
  updateBatteryStatus();

  //  Serial.println(analogRead(BATTERY_VOLTAGE_PIN));
  //for (int i = 0; i < 5; i++) {
    //digitalWrite(LED_BUILTIN, i % 2);
    for (int j = 0; j<256; j++) {
      changeColour(j, j, j);
      sodaq_wdt_safe_delay(1);
    }
    for (int j = 255; j >= 0; j--) {
      changeColour(j, j, j);
      sodaq_wdt_safe_delay(1);
    }
  //}

  digitalWrite(SLEEP_BUTTON_AND_LED, LOW);
  sodaq_wdt_safe_delay(500);


  if (isButtonPressed()) {
    sodaq_wdt_safe_delay(500);
    if (isButtonPressed()) systemSleep();
  }

}




