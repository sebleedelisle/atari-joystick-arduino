#pragma once

class Receiver { 
  public:
  Receiver() { 
    connected = false; 
    batteryLevel = 0; 
    radioLevel = 0; 
    
  }

  int id; 
  int batteryLevel; 
  int radioLevel; 
  bool connected; 

  
};
