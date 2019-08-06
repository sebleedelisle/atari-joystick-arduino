

//
const int numReadings = 1000;
int voltageReadings[numReadings];
int voltageReadingIndex = 0;

bool batteryCharging = false;
bool batteryFull = false;
int currentVoltage;  // volts*100

bool buttonPressed = false;

bool batteryCheckerInitialised = false;


bool isBatteryLow() {
  //Serial.print(analogRead(BATTERY_VOLTAGE_PIN)); Serial.print(" "); Serial.println((3.30 *1024 / 3.3/2));
  return analogRead(BATTERY_VOLTAGE_PIN) < (3.30 * 1024 / 3.3 / 2);

}
//
void initBatteryChecker() {

  for (int i = 0; i < numReadings; i++) {
    voltageReadings[i] = analogRead(BATTERY_VOLTAGE_PIN);
  }
  batteryCheckerInitialised = true;
}

int getColourForBattery() {
  Serial.print("batt : ");
  Serial.println(currentVoltage);
  if (currentVoltage >= 420) return 0x0033ff;
  else if (currentVoltage > 410) return 0x00ff00;
  else if (currentVoltage > 360) return 0xff3300;
  else return 0xff0000;

}


inline void updatePowerStatus() {
  //
  if (!batteryCheckerInitialised) {
    initBatteryChecker();
  }

  // get the battery reading
  int reading = analogRead(BATTERY_VOLTAGE_PIN);
  // now get the total of all the battery readings
  int total = 0;
  for (int i = 0; i < numReadings; i++) {
    total += voltageReadings[i];
  }
  //
  //  // get the difference between the total readings and the current reading (multiplied by the number of readings)
  //  int difference = (reading * numReadings) - total;
  //
  voltageReadings[voltageReadingIndex % numReadings] = reading;
  voltageReadingIndex++;
  float voltage = total / numReadings * 100;
  //float voltage = reading;

  //voltage*=100;
  voltage *= 2;    // we divided by 2, so multiply back
  voltage *= 3.3;  // Multiply by 3.3V, our reference voltage
  voltage /= 1024; // convert to voltage


  currentVoltage = voltage;
  if ((currentVoltage > 418) &&(!batteryFull)) {// && (difference >= -100)) { // TODO check this!
    batteryFull = true;
  } else if((currentVoltage<416) &&(batteryFull)){
    batteryFull = false;
  }
  updateChargeStatus();


}

bool updateChargeStatus() {

  pinMode(USB_VOLTAGE_PIN, INPUT);
  //lastPowerCheck = millis();
  //Serial.print("USB VOLTAGE : ");
  //Serial.println(analogRead(USB_VOLTAGE_PIN));
  if (analogRead(USB_VOLTAGE_PIN) > 700) {
    batteryCharging  = true;
  } else {
    batteryCharging  = false;
  }
  pinMode(USB_VOLTAGE_PIN, INPUT_PULLUP);
  return batteryCharging;
}

int getCurrentVoltage() { 
  return currentVoltage; 
}
bool isButtonPressed() {
  //Serial.print("BUTTON : ");
  //Serial.println(digitalRead(USB_VOLTAGE_PIN));
  //return false ; //*****************************
  return !digitalRead(USB_VOLTAGE_PIN);

}

bool isBatteryCharging() {
  return batteryCharging;
}
bool isBatteryFull() {
  return batteryFull;
}
