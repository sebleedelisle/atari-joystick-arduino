float fadeTime = 10000; 
void runRainbowEffect() {

  unsigned long elapsed = millis();//- effectStartTime;

  int hue = (int)((unsigned long)(elapsed) * 0.4f) % 360;
  for (int i = 0; i < numShoes; i++) {

    H2R_HSBtoRGB( (hue + i * 30) % 360,  100, 100, rgb);
    addColourChangeMessageToBuffer(shoeIDs[i], rgb[0], rgb[1], rgb[2]);

  }
  sendMessages();



}


void runRainbowFadeEffect() {

  unsigned long elapsed = millis() - effectStartTime;
  int brightness = ((fadeTime-elapsed)*100)/fadeTime; 
  if(brightness<0) brightness = 0; 

  int hue = (int)((unsigned long)(millis()) * 0.4f) % 360;
  for (int i = 0; i < numShoes; i++) {

    H2R_HSBtoRGB( (hue + i * 30) % 360,  100, brightness, rgb);
    addColourChangeMessageToBuffer(shoeIDs[i], rgb[0], rgb[1], rgb[2]);

  }
  sendMessages();



}
