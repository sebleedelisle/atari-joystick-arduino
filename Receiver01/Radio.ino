#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>
//#include "Receiver.h"

#define BROADCAST_ID 255
#define RF69_FREQ 434.0

#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4

//server address - can we stop this being hard coded?
#define MY_ADDRESS     0

uint8_t key[] = { 'S', 'n', 'o', '0', 'p', 'Y', 0x07, 0x08,
                  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
                };

uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

uint16_t bufferIndex = 0;
uint8_t numMessages = 0;

RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);

void resetBuffer() {
  bufferIndex = 4;
  numMessages = 0;
}

void setupRadio() {

  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather Addressed RFM69 TX Test!");
  Serial.println();

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69_manager.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }

  rf69_manager.setTimeout(20);
  rf69_manager.setRetries(1);
  Serial.println("RFM69 radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }


  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server

  rf69.setEncryptionKey(key);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
  //  Serial.print("Address set to : "); Serial.println(rf69_manager.thisAddress());

  resetBuffer();

}

void checkReceiverStatus(Receiver& shoe) {
  uint8_t command = 's';
  //Serial.println(command);
  if (rf69_manager.sendtoWait((uint8_t *)&command, 1, shoe.id)) {

    //checkInputs()

    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
      //checkInputs()
      Serial.print("received from "); Serial.print(shoe.id); Serial.println(" : ");
      //Serial.println(buf[0]);
      shoe.batteryLevel = buf[0] << 1;
      Serial.print("Battery : ");
      Serial.println(shoe.batteryLevel);
      shoe.radioLevel = (int8_t)buf[1];
      Serial.print("Radio   : "); 
      Serial.println(shoe.radioLevel); 
      shoe.connected = true;

    } else {

      Serial.print("no response from "); Serial.print(shoe.id); Serial.println(".");
      shoe.connected = false;
    }

  } else {
    Serial.print("no response from "); Serial.print(shoe.id); Serial.println(".");
    shoe.connected = false;
  }
}


void addColourChangeMessageToBuffer(uint8_t dest, int colour) {
  addColourChangeMessageToBuffer(dest, (colour >> 16) & 0xFF,  (colour >> 8) & 0xFF, colour & 0xFF);

}

void addColourChangeMessageToBuffer(uint8_t dest, uint16_t r, uint16_t g, uint16_t b) {


  outputStrip.setPixelColor(getShoeIndexByID(dest), r >> 4, g >> 4, b >> 4);

  uint16_t& i = bufferIndex;

  buf[i++] = 'c'; // colour change message
  buf[i++] = dest;  // recipient id
  //buf[i++] = r>>4;
  buf[i++] = r & 0xff;
  //buf[i++] = g>>4;
  buf[i++] = g & 0xff;
  //buf[i++] = b>>4;
  buf[i++] = b & 0xff;

  buf[2] = i >> 4;
  buf[3] = i & 0xff;

  numMessages++;
  //  Serial.println("---");
  //  Serial.println(i, HEX);
  //  Serial.println(i >> 4, HEX);
  //  Serial.println(i & 0xf, HEX);
  //  Serial.println(buf[2] << 4 | buf[3], HEX);
  //  Serial.println("---");

}

void sendMessages() {

  buf[0] = 0; // sender id
  buf[1] = numMessages; // number of messages
  // buf 2-3 are the number of bytes

  buf[2] = bufferIndex >> 4;
  buf[3] = bufferIndex & 0xff;

  rf69_manager.sendtoWait(buf, bufferIndex, RH_BROADCAST_ADDRESS);
  rf69_manager.waitPacketSent();

  resetBuffer();
  outputStrip.show();
}

//
//void send(char* packet) {
//
//  rf69.send((uint8_t*)packet, 7);
//  rf69.waitPacketSent();
//
//}

void checkMessages () {

  if (rf69.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;

    if (rf69.recv(buf, &len)) {
      buf[len] = 0; // zero out remaining string

      //      Serial.print("Got packet from #"); Serial.print(from);
      //      Serial.print(" [RSSI :");
      //      Serial.print(rf69.lastRssi());
      //      Serial.print("] : ");
      //      Serial.println((char*)buf);


      // Send a reply back to the originator client
      //if (!rf69_manager.sendtoWait(buf, sizeof(buf), from))
      // Serial.println("Sending failed (no ack)");
    }
  }

  //  if (rf69_manager.available())
  //  {
  //    // Wait for a message addressed to us from the client
  //    uint8_t len = sizeof(buf);
  //    uint8_t from;
  //    if (rf69_manager.recvfromAck(buf, &len, &from)) {
  //      buf[len] = 0; // zero out remaining string
  //
  //      Serial.print("Got packet from #"); Serial.print(from);
  //      Serial.print(" [RSSI :");
  //      Serial.print(rf69.lastRssi());
  //      Serial.print("] : ");
  //      Serial.println((char*)buf);
  //      // Blink(LED, 40, 3); //blink LED 3 times, 40ms between blinks
  //
  //      // Send a reply back to the originator client
  //      //if (!rf69_manager.sendtoWait(data, sizeof(data), from))
  //      //Serial.println("Sending failed (no ack)");
  //    }
  //  }

}
