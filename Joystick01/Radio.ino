

// Messages from sender: 
// 's' - status - send status back
// 'z' - snooze - go into snooze mode
// 'p' - sleep  - go into sleep mode
// 'w' - wake   - wake up!

// multiple message format for quickly sending messages to all shoes: 
// | byte 0  |        byte 1      |       byte 2      |     byte 3     |
// | sender  | number of commands |  number of bytes                   |
// colour command
// 'c', recipient, r, g, b, 


#define RF69_FREQ 434.0

// Feather M0 w/Radio
#define RFM69_CS      8
#define RFM69_INT     3
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

void updateStatusData() {
  statusData[0] = min(currentVoltage / 2, 254);
  statusData[1] = min(rf69.lastRssi(), 254);
  Serial.print("updateStatusData : v:");
  Serial.print(currentVoltage); 
  Serial.print(" v:"); 
  Serial.print(statusData[0]); 
  Serial.print(" r:");
  Serial.print(rf69.lastRssi()); 
  Serial.print(" r:"); 
  Serial.println(statusData[1]); 
  
}
void checkMessages () {

  while (rf69_manager.available()) {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t sender;
    //Serial.println(len);
    while (rf69_manager.recvfromAck(buf, &len, &sender)) {
      //Serial.println(buf[0]);

      // if it's a status check message
      if ((len == 1) && (buf[0] == 's')) {
        updateStatusData();
        Serial.print(statusData[0]);
        Serial.print(" ");
        Serial.println(statusData[1]);
        if (!rf69_manager.sendtoWait(statusData, sizeof(statusData), sender))
          Serial.println("Sending failed (no ack)");

      } else {
        // otherwise it's a big message for all the shoes
        lastCommandTime = millis(); 
        // len is updated with the message length

        int i = 0;
        uint8_t sender = buf[i++];
        uint8_t messageNum = buf[i++];
        uint16_t byteNum = buf[i++] << 4 | buf[i++];

        // to do : error check with number of bytes

        while (i < len) {

          char command = buf[i++];
//          Serial.print(i);
//          Serial.print(" sender : "); Serial.print(sender);
//          Serial.print(" message num :"); Serial.print(messageNum);
//          Serial.print(" byte num :"); Serial.print(byteNum);
//          Serial.print(" command :"); Serial.print(command);
//          Serial.println("");

          if (command == 'c') {
            uint8_t target = buf[i++];
//            Serial.print(" target :"); Serial.print(target);
//            Serial.println("");

            uint8_t r = buf[i++];
            uint8_t g = buf[i++];
            uint8_t b = buf[i++];
            if ((target == BROADCAST_ID) || (target == myID)) {
              changeColour(r, g, b);
//              Serial.print(" colour : ");
//              Serial.print(r, HEX); Serial.print(",");
//              Serial.print(g, HEX); Serial.print(",");
//              Serial.print(b, HEX);
//              Serial.println("");
            }

          }

        }
      }
    }
  }
}

bool processMessage() {

  // header is 4 bytes :
  // 0 : sender id
  // 1 : number of commands
  // 2 : buffer length



  return true;

}
