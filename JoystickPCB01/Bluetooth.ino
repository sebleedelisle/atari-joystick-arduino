#ifdef USE_BLUETOOTH

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

/*=========================================================================
    APPLICATION SETTINGS

      FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
     
                                Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                                running this at least once is a good idea.
     
                                When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                                Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
         
                                Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    -----------------------------------------------------------------------*/
#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
/*=========================================================================*/

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setupRadio() {
  
//  /* Disable command echo from Bluefruit */
//  ble.echo(false);
//
//  Serial.println("Requesting Bluefruit info:");
//  /* Print Bluefruit information */
//  ble.info();
//
//  /* Change the device name to make it easier to find */
//  Serial.println(F("Setting device name to 'Bluefruit Keyboard': "));
//  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Bluefruit Keyboard" )) ) {
//    error(F("Could not set device name?"));
//  }
//
//  /* Enable HID Service */
//  Serial.println(F("Enable HID Service (including Keyboard): "));
//  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
//  {
//    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
//      error(F("Could not enable Keyboard"));
//    }
//  } else
//  {
//    if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
//      error(F("Could not enable Keyboard"));
//    }
//  }
//
//  /* Add or remove service requires a reset */
//  Serial.println(F("Performing a SW reset (service changes require a reset): "));
//  if (! ble.reset() ) {
//    error(F("Couldn't reset??"));
//  }
//
//  Serial.println();
//  Serial.println(F("Go to your phone's Bluetooth settings to pair your device"));
}

bool sendStatus(uint8_t statusflags) {

  for (int i = 0; i < 5; i++) {
    //if(
    //ble.print("AT+BleKeyboard=");
    //ble.println(keys);

  }
  
  

//  if ( ble.waitForOK() )
//  {
//    Serial.println( F("OK!") );
//  } else
//  {
//    Serial.println( F("FAILED!") );
//  }

  return true;
}

#endif
