// rf69 demo tx rx.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF69 class. RH_RF69 class does not provide for addressing or
// reliability, so you should only use RH_RF69  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf69_server.
// Demonstrates the use of AES encryption, setting the frequency and modem
// configuration

#include <SPI.h>
#include <RH_RF69.h>

/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 434.0

#if defined(ARDUINO_SAMD_FEATHER_M0) // Feather M0 w/Radio
#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4
#endif



// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

int16_t packetnum = 0;  // packet counter, we increment per xmission


const int greenLEDPin = A0;    // LED connected to digital pin 9
const int redLEDPin = A1;     // LED connected to digital pin 10
const int blueLEDPin = A2;    // LED connected to digital pin 11


void setup()
{
  Serial.begin(115200);
  //while (!Serial) { delay(1); } // wait until serial console is open, remove if not tethered to computer

  pinMode(redLEDPin, OUTPUT); 
  pinMode(greenLEDPin, OUTPUT); 
  pinMode(blueLEDPin, OUTPUT); 

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather RFM69 RX Test!");
  Serial.println();

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
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
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
                  };
  rf69.setEncryptionKey(key);

  //pinMode(LED, OUTPUT);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
}


void loop() {

  changeColour(255,255,255); 
  delay(2000); 
  digitalWrite(LED_BUILTIN,HIGH); 
  changeColour(0,0,0); 
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  return; 
  for(int i = 255; i>=0; i--) {
    changeColour(i,0,0);
    delay(10);  
  }
for(int i = 255; i>=0; i--) {
    changeColour(0,i,0);
    delay(10);  
  }
for(int i = 255; i>=0; i--) {
    changeColour(0,0,i);
    delay(10);  
  }
for(int i = 255; i>=0; i--) {
    changeColour(i,i,i);
    delay(10);  
  }

  
  
  delay(1000); 

  
//  if (rf69.available()) {
//    // Should be a message for us now
//    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
//    uint8_t len = sizeof(buf);
//    if (rf69.recv(buf, &len)) {
//      if (len==0) return;
//      buf[len] = 0;
//      Serial.print("Received [");
//      Serial.print(RH_RF69_MAX_MESSAGE_LEN);
//      Serial.print("]: ");
//      Serial.println((char*)buf);
//      Serial.print("RSSI: ");
//      Serial.println(rf69.lastRssi(), DEC);
//
//      long colour = strtol((char*)buf, NULL, 16); 
//  
//
//      changeColour(colour); 
//      
//      if (strstr((char *)buf, "0")) {
//        digitalWrite(LED, LOW);
//        //        // Send a reply!
//        //        uint8_t data[] = "And hello back to you";
//        //        rf69.send(data, sizeof(data));
//        //        rf69.waitPacketSent();
//        //        Serial.println("Sent a reply");
//        //        Blink(LED, 40, 3); //blink LED 3 times, 40ms between blinks
//      } else if (strstr((char *)buf, "1")) {
//        digitalWrite(LED, HIGH);
//      }
//    }
//    //    } else {
//    //      Serial.println("Receive failed");
//    //    }
//  }
}


void changeColour(int colour) {

      changeColour((colour>>16), (colour>>8 & 0xff), (colour & 0xff));  



}



void changeColour(int r, int g, int b) {
  float bright = 1; 
  analogWrite(redLEDPin, r*bright);
  analogWrite(greenLEDPin, g*bright);
  analogWrite(blueLEDPin, b*bright);


}

