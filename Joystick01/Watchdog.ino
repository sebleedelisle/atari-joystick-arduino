
void setupWatchdog() {
  // initialise watchdog timer to interupt every 4 seconds
  //sodaq_wdt_enable(WDT_PERIOD_4X);
}




void systemSleep()
{

  

  changeColour(0);

 
  if (true) 
  {
    Serial.println("Ready to sleep");
    // Power off all LEDs before sleep
    for (int j = 100; j >= 0; j -= 2) {
      changeColour(j, 0, 0);
      sodaq_wdt_safe_delay(1);
    }
    digitalWrite(LED_PIN, LOW);
    rf69.sleep();

    // wait until user releases button
    while (isButtonPressed()) sodaq_wdt_safe_delay(1);

    //Serial.end();

    // Disable USB but only if we're not monitoring with serial
    if (!Serial) {
      USBDevice.detach();
    }

    // Set the sleep mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    boolean awake = false;


    // two states : SLEEP and SNOOZE, only difference is that if we're in SLEEP we
    // send out a radio message less frequently. Let's just implement SNOOZE for now.
    // SNOOZE : Every 8 seconds on an interupt:
    // check the button...
    // if pressed, wait 1 second and check again
    // if still pressed, wake up
    // otherwise :
    // send out a radio message.
    // if no reply or a snooze reply, then do nothing.
    // if wake message received then wake up

    // if we're charging then flash the shoe LEDS

    while (!awake ) {

      // SAMD sleep

      __WFI();

      //      if (!(batteryFull && batteryCharging)) {
      //        digitalWrite(SLEEP_BUTTON_AND_LED, LOW);
      //        __WFI();
      //      }
      //      else {
      //        digitalWrite(SLEEP_BUTTON_AND_LED, HIGH);
      //        sodaq_wdt_safe_delay(1000);
      //      }

      if (isButtonPressed()) {
        sodaq_wdt_safe_delay(1000);
        if (isButtonPressed()){
          awake = true;
          Serial.println("BUTTON PRESSED");
//          digitalWrite(SLEEP_BUTTON_AND_LED, HIGH);
//          sodaq_wdt_safe_delay(50);
//          digitalWrite(SLEEP_BUTTON_AND_LED, LOW);
//          sodaq_wdt_safe_delay(50);
//          digitalWrite(SLEEP_BUTTON_AND_LED, HIGH);
//          sodaq_wdt_safe_delay(50);
//          digitalWrite(SLEEP_BUTTON_AND_LED, LOW);
//          sodaq_wdt_safe_delay(50);
//          digitalWrite(SLEEP_BUTTON_AND_LED, HIGH);
//          sodaq_wdt_safe_delay(50);
//          digitalWrite(SLEEP_BUTTON_AND_LED, LOW);
//          sodaq_wdt_safe_delay(50);
        }

      }
      if(updateChargeStatus()) { 
        awake = true; 
      }

      if (!awake) {
        // send out message...
        // wait for reply
        // if wake message, then wake up
      }

      //updateBatteryStatus();


      if (!awake)  { // if we're charging - how do we know?
        //        if (batteryFull && batteryCharging) { // if we're full, then just light up green
        //
        //          changeColour(0, 2, 0);
        //
        //          //changeColour(0, 0, 0);
        //
        //        } else if (batteryCharging) { // low power so actively charging, flash green
        //          changeColour(0, 5, 0);
        //          digitalWrite(SLEEP_BUTTON_AND_LED, HIGH);
        //          sodaq_wdt_safe_delay(50);
        //          changeColour(0);
        //          digitalWrite(SLEEP_BUTTON_AND_LED, LOW);
        //        } else {
        changeColour(0);
        digitalWrite(LED_PIN, HIGH);
        sodaq_wdt_safe_delay(5);
        digitalWrite(LED_PIN, LOW);
        //        }

      }
      sodaq_wdt_safe_delay(50);
    }


    //changeColour(255, 255, 0);
    // wait until button released
    //while (isButtonPressed()) sodaq_wdt_safe_delay(1);

    // reboot on wake
    NVIC_SystemReset();
  }
  */

}
