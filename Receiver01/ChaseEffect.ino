
int chaseFrameNum = 0;

float freq = 1.0f / 140.0f * 60.0f * 1000.0f  / 4.0f ; // TODO sync with 130 BPM
//float brightnesses[] = {0, 0, 0, 0, 0, 0};
int shoeFlash[] = {shoeIDs[1], shoeIDs[0],shoeIDs[5], shoeIDs[4], shoeIDs[3], shoeIDs[2]};
//int shoeFlash[] = {1, 2, 3, 4, 36, 37};

//
//void runChaseEffect() {
//  unsigned long elapsed = millis() - effectStartTime;
//
//  int frame = elapsed / freq;
//  float frameprogress = float(fmod(elapsed, (freq*2))) / (freq*2);
//
//  int flashShoe = frame % 2;
//
//  for (int i = 0; i < 6; i++) {
//    //brightnesses[i] -= 5;
//    //if (brightnesses[i] < 0) brightnesses[i] = 0;
//    //if (i % 2 == flashShoe) {
//      //brightnesses[i] = 255;
//      int b = (1.0f-frameprogress) * 255;
//      addColourChangeMessageToBuffer(shoeFlash[i], b, b, b);
//    //} else
//    //  addColourChangeMessageToBuffer(shoeFlash[i], 0,0,0);
//  }
//
//  sendMessages();
//
//}


void runChaseEffect() {
  unsigned long elapsed = millis() - effectStartTime;

  int frame = elapsed / freq;
  float frameprogress = float(fmod(elapsed, freq)) / freq;

  int flashShoe = frame % 4;

  for (int i = 0; i < 6; i++) {
    //brightnesses[i] -= 5;
    //if (brightnesses[i] < 0) brightnesses[i] = 0;
    if (i%4 == flashShoe) {
      //brightnesses[i] = 255;
      int b = frameprogress > 0.25 ? 0 : 255;
      addColourChangeMessageToBuffer(shoeFlash[i], b, b, b);
    } else {
      addColourChangeMessageToBuffer(shoeFlash[i], 0,0,0);
    }
  }

  sendMessages();

}
