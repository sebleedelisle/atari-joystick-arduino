 #include <inttypes.h>
#include "HardwareSerial.h"

// 12 Step Rotary Encoder with Click //
// http://www.sparkfun.com/products/9117 //
#define EncoderPinA 21  // Rotary Encoder Left Pin //
#define EncoderPinB 20  // Rotary Encoder Right Pin //
#define EncoderPinP 10 // Rotary Encoder Click //

// ======================================================================================= //
class Encoder
{
  public:
    Encoder()
    {
      pinMode(EncoderPinA, INPUT);
      pinMode(EncoderPinB, INPUT);
      Pos = 2; 
      Position = 0;
      Position2 = 0;

    }

    void tick(void)
    {
      Position2 = (digitalRead(EncoderPinB) * 2) + digitalRead(EncoderPinA);;
      if (Position2 != Position)
      {
        isFwd = ((Position == 0) && (Position2 == 1)) || ((Position == 1) && (Position2 == 3)) ||
                ((Position == 3) && (Position2 == 2)) || ((Position == 2) && (Position2 == 0));
//        if (!digitalRead(EncoderPinP)) {
//          if (isFwd) Pos += clickMultiply;
//          else Pos -= clickMultiply;
//        }
//        else {
          if (isFwd) Pos++;
          else Pos--;
//        }
        //if (Pos < Min) Pos = Min;
        //if (Pos > Max) Pos = Max;
      }
      Position = Position2;
    }

    int getPos(void)
    {
      return (Pos / 4);
    }

   

  private:
  
    volatile int Pos;
    int Position;
    int Position2;
    int isFwd;
};
