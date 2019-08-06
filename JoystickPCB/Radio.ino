
#ifndef USE_BLUETOOTH


#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#define RF69_FREQ 868

// Feather M0 w/Radio
#define RFM69_CS      8
#define RFM69_INT     7
#define RFM69_RST     4

// The encryption key has to be the same as the one in the server
uint8_t key[] = { 'S', 'n', 'o', '0', 'p', 'Y', 0x07, 0x08,
                  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
                };
//
//#define MY_ADDRESS     1
#define BROADCAST_ID 255

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);
RHReliableDatagram rf69_manager(rf69, getId());

// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

uint8_t senderId = 0;
uint8_t numberOfCommands = 0;
uint16_t numberOfBytes = 0;


uint8_t statusData[] = {255, 255}; // 2 bytes, one for battery, one for radio strength


void setupRadio() {
  myID = getId();
  Serial.print("ID : ");  Serial.println(myID);
  rf69_manager.setThisAddress(myID);

  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // reset radio
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);


  if (!rf69_manager.init()) {
    Serial.println("RFM69 radio init failed");
    // TO DO : FLASH ERROR
    while (1);
  }

  rf69_manager.setTimeout(20);
  rf69_manager.setRetries(1);

  // turn this on to save some battery!
  //rf69.setIdleMode(RH_RF69_OPMODE_MODE_SLEEP);

  Serial.println("RFM69 radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
    // TO DO : FLASH ERROR
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW



  rf69.setEncryptionKey(key);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");


}

bool sendStatus(uint8_t statusflags) {

  //printBits(statusflags);
  bool sendsuccess = false; 
  if (rf69_manager.sendtoWait(&statusflags, 1, RECEIVER_ADDRESS)) {
    //Serial.println("SEND SUCCESS!"); 
    sendsuccess = true; 
    
  } else { 
    //Serial.println("SEND FAIL"); 
  }
  lastMessageTime = millis(); 
  return sendsuccess; 
  

  return true;
}

#endif
void printBits(byte myByte) {
  for (byte mask = 0x80; mask; mask >>= 1) {
    if (mask  & myByte)
      Serial.print('1');
    else
      Serial.print('0');
  }
  Serial.println(' ');
}
