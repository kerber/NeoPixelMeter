/*
  NeoPixelMeter.h - Library for adding a Neopixel meter to your projects.
  Created by Dustin kerber, March 3, 2014.
  Released into the public domain.
*/
#ifndef NeoPixelMeter_h
#define NeoPixelMeter_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <Adafruit_NeoPixel.h>

#define ANIMATION_POUR  0 //Pouring animation
#define ANIMATION_LEVEL  1 //Level gauge animation
#define ANIMATION_LEVEL2  2 //Level gauge animation

class NeoPixelMeter
{
  public:
    //Create a new meter display set.
    //WARNING: Consult Neopixel uberguide for power supply requirements and wiring best practices.
    //http://learn.adafruit.com/adafruit-neopixel-uberguide/overview
    //Arg dataPin:  Arduino data pin connected to NeoPixel data input.
    //Arg maxBrightness:  (0-255) Maximum brightness level for the meter set
    //Arg numMeters:  Number of meters chained together.
    //Arg pixelsPerMeter:  Number of pixels on each meter.
    NeoPixelMeter(int dataPin, int maxBrightness, int numMeters, int pixelsPerMeter, byte ledType = NEO_GRB + NEO_KHZ800);

    void begin();

    //Update the Neopixel chain. Should be called in loop.
    void displayDraw();

    //Set the level illumnited for each meter
    //Arg meterNum : 0=First meter in chain, 1=Second meter in chain, etc.
    //Arg percentFull:  (0-100) Level of meter illuminated
    void setLevel(int meterNum, int percentFull);

    //Set a meters's animation directly.
    //Arg meterNum : 0=First meter in chain, 1=Second meter in chain, etc.
    //Arg animation:  One of the predefined constant animation types
    void setAnimation(int meterNum, int animation);

    //When awake, the meter should show the scrolling animation
    //Arg meterNum:  0=First meter in chain, 1=Second meter in chain, etc.
    void showScrolling(int meterNum);

    //When awake, the meter should show the level as a gauge
    //Arg meterNum:  0=First meter in chain, 1=Second meter in chain, etc.
    void showLevel(int meterNum);

    //Fade to new brightness
    //Arg brightness:  (0-255) Target light level
    void setBrightnessWithFade(int brightness);

    //Set new brightness
    //Arg brightness:  (0-255) Target light level
    void setBrightness(int brightness);

    //Fade the meter on.
    //Calling while already awake will reset the timer.
    //Arg secondsAwake: The number of seconds to wait before automatically calling displaySleep().
    //                  If set to 0, keep the display on until manually calling displaySleep().
    void displayWakeup(int secondsAwake);

    //Set displays off
    void displaySleep();

  private:
    const byte
      _dataPin,
      _maxBrightness,
      _numMeters,
      _pixelsPerMeter;
    byte currentBrightness;
    byte targetBrightness;
    byte animationType[10];
    byte meterLevel[10];
    int16_t animationCounter[10];
    void hsi2rgb(float H, float S, float I, int* rgb);
    unsigned int fadeDelay;
    unsigned int fadeTimer;
    unsigned long sleepTime;
    unsigned long lastDisplayMillis;
    Adafruit_NeoPixel pixels;
};

#endif
