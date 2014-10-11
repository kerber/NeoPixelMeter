#include <NeoPixelMeter.h>
#include <Adafruit_NeoPixel.h>


NeoPixelMeter meters = NeoPixelMeter(2, 100, 1, 16, NEO_GRB + NEO_KHZ800);
unsigned long actionTime = 0;
byte counter = 100;
byte animCounter = 0;

void setup() {
  meters.begin();
  meters.setLevel(0,100);
  meters.showLevel(0);
  meters.displayWakeup(0);
}

void loop() {
  unsigned long nowT = millis();
  meters.displayDraw();
  if ( actionTime == 0 || actionTime + 90 <= nowT )
  {
    actionTime = nowT;
    if (counter == 0)
    {
      animCounter ++;
      meters.setAnimation(0,animCounter%3);
      counter = 100;
    }
    else
    {
      counter -= 1;
    }
    meters.setLevel(0,counter);
  }
}
