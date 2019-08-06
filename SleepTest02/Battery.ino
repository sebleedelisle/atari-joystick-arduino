


const int numReadings = 100;
int voltageReadings[numReadings];
int voltageReadingIndex = 0;
bool batteryCharging = false;
bool batteryFull = false; 
int currentVoltage;  // volts*100

bool batteryCheckerInitialised = false;

void initBatteryChecker() {
  for (int i = 0; i < numReadings; i++) {
    voltageReadings[i] = analogRead(BATTERY_VOLTAGE_PIN);
  }
  batteryCheckerInitialised = true;
}

inline void updateBatteryStatus() {

  if (!batteryCheckerInitialised) {
    initBatteryChecker();
  }
  int reading = analogRead(BATTERY_VOLTAGE_PIN);

  int total = 0;
  for (int i = 0; i < numReadings; i++) {
    total += voltageReadings[i];
  }

  int difference = (reading * numReadings) - total; 
  //batteryCharging = difference>100;

  float usbvoltage = analogRead(USB_VOLTAGE_PIN)*200*3.3/1024; 
  batteryCharging = usbvoltage>=480; 

  voltageReadings[voltageReadingIndex % numReadings] = reading;
  voltageReadingIndex++;
  float voltage = total/numReadings*100; 
  
  voltage *= 2;    // we divided by 2, so multiply back
  voltage *= 3.3;  // Multiply by 3.3V, our reference voltage
  voltage /= 1024; // convert to voltage
  currentVoltage = voltage; 
  if((currentVoltage>418) && (difference>=-100)) batteryFull = true; 
  else batteryFull = false; 
  
  Serial.print("usbv: " ); Serial.print(usbvoltage); Serial.print(" c: ");Serial.print(batteryCharging);  
  Serial.print(" f: ");Serial.print(batteryFull);  
  Serial.print(" reading: " ); Serial.print((reading * numReadings)); 
  Serial.print(" total: " ); Serial.print(total); Serial.print(" diff : "); Serial.println(difference);

}

