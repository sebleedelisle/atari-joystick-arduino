
const int pin0 = 12;
const int pin1 = 11;
const int pin2 = 10;
const int pin3 = 9;

uint8_t getId() {

  //    pinMode(12, INPUT);
  //    Serial.println(digitalRead(pin0));

  

  uint8_t bit0 = getValueForPinPair(pin0, pin1);
  uint8_t bit1 = getValueForPinPair(pin1, pin2);
  uint8_t bit2 = getValueForPinPair(pin2, pin3);
  
  // Serial.print("ID : ");
  //Serial.println(bit0 | (bit1<<1) | (bit2<<2), BIN);
  //  Serial.println(bit1, BIN);
  //  Serial.println(bit2, BIN);
  //
  return bit0 | (bit1 << 1) | (bit2 << 2)  ;

}

uint8_t getValueForPinPair(int p1, int p2) {

  bool forward = false;
  bool backward = false;
  
  pinMode(p2, INPUT);
  digitalWrite(p2, HIGH); // PULL UP

  pinMode(p1, OUTPUT);
  digitalWrite(p1, LOW);

  sodaq_wdt_safe_delay(1);

  if (digitalRead(p2) == LOW) backward = true;

  
  pinMode(p1, INPUT);
  digitalWrite(p1, HIGH);
  pinMode(p2, OUTPUT);
  digitalWrite(p2, LOW);

  sodaq_wdt_safe_delay(1);
  if (digitalRead(p1) == LOW) forward = true;

  pinMode(p1, INPUT); 
  pinMode(p2, INPUT); 
  //
//    Serial.print("p1 :");Serial.println(p1);
//    Serial.print("p 2 :");Serial.println(p2);
//    Serial.print("forward :");Serial.println(forward);
//    Serial.print("backward :");Serial.println(backward);
//    Serial.print("LS bit :"); Serial.println(forward?1:0);
//    Serial.print("MS bit :");Serial.println(((forward!=backward)?8:0));
//    Serial.print("pairresult : "); Serial.println((forward?1:0) | ((forward!=backward)?8:0));

  //  uint8_t result = (forward?1:0) | ((forward!=backward)?8:0);
  return (forward ? 1 : 0) | ((forward != backward) ? 8 : 0);


}
