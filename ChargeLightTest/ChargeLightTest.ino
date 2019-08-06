


void setup() {
  // put your setup code here, to run once:
  pinMode(A3, INPUT); 
  pinMode(LED_BUILTIN, OUTPUT); 
  
}


void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, digitalRead(A3)); 
  Serial.println(analogRead(A3)); 
}
