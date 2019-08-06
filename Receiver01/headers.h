#include "Receiver.h"
void setupRadio();
void runChaseEffect(); 
void runRainbowEffect();
void runRainbowFadeEffect();
void addColourChangeMessageToBuffer(uint8_t dest, uint16_t r, uint16_t g, uint16_t b);
void sendMessages();
void checkReceiverStatus(Receiver& shoe); 
