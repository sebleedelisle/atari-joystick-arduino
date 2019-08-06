#define BATTERY_VOLTAGE_PIN A9

//
//const int numReadings = 1000;
//int voltageReadings[numReadings];
//int voltageReadingIndex = 0;
//
float currentVoltage = 0;  // volts*100
//
//bool batteryCheckerInitialised = false;


uint32_t getColourForBattery() {
  float reading = (float)analogRead(BATTERY_VOLTAGE_PIN) *2 * 330          / 1024; 
  if(currentVoltage == 0 ) currentVoltage = reading; 
  else currentVoltage+=(((float)reading-currentVoltage)*0.05f); 
  //Serial.print("batt : ");
  //   Serial.println(currentVoltage);

  if (currentVoltage > 400) return 0x00ff00;
  else if (currentVoltage > 370) return 0x66ff00;
  else return 0xff0000;

}


//
//bool isBatteryLow() {
//  //Serial.print(analogRead(BATTERY_VOLTAGE_PIN)); Serial.print(" "); Serial.println((3.30 *1024 / 3.3/2));
//  return analogRead(BATTERY_VOLTAGE_PIN) < (3.30 * 1024 / 3.3 / 2);
//
//}
////
//void initBatteryChecker() {
//
//  for (int i = 0; i < numReadings; i++) {
//    voltageReadings[i] = analogRead(BATTERY_VOLTAGE_PIN);
//  }
//  batteryCheckerInitialised = true;
//}

//
//
//inline void updatePowerStatus() {
//  //
//  if (!batteryCheckerInitialised) {
//    initBatteryChecker();
//  }
//
//  // get the battery reading
//  int reading = analogRead(BATTERY_VOLTAGE_PIN);
//  // now get the total of all the battery readings
//  int total = 0;
//  for (int i = 0; i < numReadings; i++) {
//    total += voltageReadings[i];
//  }
//  //
//  //  // get the difference between the total readings and the current reading (multiplied by the number of readings)
//  //  int difference = (reading * numReadings) - total;
//  //
//  voltageReadings[voltageReadingIndex % numReadings] = reading;
//  voltageReadingIndex++;
//  float voltage = total / numReadings * 100;
//  //float voltage = reading;
//
//  //voltage*=100;
//  voltage *= 2;    // we divided by 2, so multiply back
//  voltage *= 3.3;  // Multiply by 3.3V, our reference voltage
//  voltage /= 1024; // convert to voltage
//
//
//  currentVoltage = voltage;
//  if ((currentVoltage > 418) &&(!batteryFull)) {// && (difference >= -100)) { // TODO check this!
//    batteryFull = true;
//  } else if((currentVoltage<416) &&(batteryFull)){
//    batteryFull = false;
//  }
//  updateChargeStatus();
//
//
//}
//
//bool updateChargeStatus() {
//
//  pinMode(USB_VOLTAGE_PIN, INPUT);
//  //lastPowerCheck = millis();
//  //Serial.print("USB VOLTAGE : ");
//  //Serial.println(analogRead(USB_VOLTAGE_PIN));
//  if (analogRead(USB_VOLTAGE_PIN) > 700) {
//    batteryCharging  = true;
//  } else {
//    batteryCharging  = false;
//  }
//  pinMode(USB_VOLTAGE_PIN, INPUT_PULLUP);
//  return batteryCharging;
//}
//
//int getCurrentVoltage() { 
//  return currentVoltage; 
//}
//bool isButtonPressed() {
//  //Serial.print("BUTTON : ");
//  //Serial.println(digitalRead(USB_VOLTAGE_PIN));
//  //return false ; //*****************************
//  return !digitalRead(USB_VOLTAGE_PIN);
//
//}
//
//bool isBatteryCharging() {
//  return batteryCharging;
//}
//bool isBatteryFull() {
//  return batteryFull;
//}
