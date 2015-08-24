
#include <Adafruit_NeoPixel.h>
#include <NeoPixelMeter.h>
#include "animation.h"
#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

NeoPixelMeter::NeoPixelMeter(int dataPin, int maxBrightness, int numMeters,
  int pixelsPerMeter, byte ledType) : _dataPin(dataPin),
  _maxBrightness(maxBrightness), _numMeters(numMeters), _pixelsPerMeter(pixelsPerMeter),
  pixels(_numMeters * _pixelsPerMeter, _dataPin, NEO_GRB + NEO_KHZ800)
{
  //TODO: Set top pixel in initilization
}

void NeoPixelMeter::begin()
{
  pixels.begin();
}

void NeoPixelMeter::setLevel(int meterNum, int percentFull)
{
  meterLevel[meterNum] = percentFull;
}

void NeoPixelMeter::setAnimation(int meterNum, int animation)
{
  animationType[meterNum] = animation;
}

void NeoPixelMeter::showScrolling(int meterNum)
{
//  animationCounter[meterNum] = 0;
  animationType[meterNum] = ANIMATION_POUR;
}

void NeoPixelMeter::showLevel(int meterNum)
{
//  animationCounter[meterNum] = 0;
  animationType[meterNum] = ANIMATION_LEVEL;
}

void NeoPixelMeter::setBrightnessWithFade(int brightness)
{
  targetBrightness = brightness;
}

void NeoPixelMeter::setBrightness(int brightness)
{
  sleepTime = 0;
  targetBrightness = brightness;
  currentBrightness = brightness;
}

void NeoPixelMeter::displayWakeup(int secondsAwake)
{
  if ( secondsAwake > 0 )
  {
    sleepTime = millis() + secondsAwake * 1000;
  }
  else
  {
    sleepTime = 0;
  }
  targetBrightness = _maxBrightness;
}

void NeoPixelMeter::displaySleep()
{
  targetBrightness = 0;
}

void NeoPixelMeter::displayDraw()
{
  unsigned long nowTime = millis();
  if ( (lastDisplayMillis + 15) <= nowTime )
  {
    if ( sleepTime != 0 && sleepTime <= nowTime )
    {
      //check if the display should be going to sleep
      //if it should be sleeping, set the targetBrightness to 0 to start fa
      targetBrightness = 0;
      sleepTime = 0;
    }
    if ( currentBrightness < targetBrightness )
    {
      currentBrightness ++;
    }
    else if ( currentBrightness > targetBrightness )
    {
      currentBrightness --;
    }
    if (currentBrightness > 0)
    {
      uint8_t r, g, b, a, c, d;
      byte numGraphPixels;
      byte pointsPerPixel;
      byte numLitPixels;
      int rgb[3];

      for ( int meterNum = 0; meterNum < _numMeters; meterNum++ )
      {
        hsi2rgb(meterLevel[meterNum]*119/100, 1, 1, rgb);
        switch (animationType[meterNum])
        {
          case ANIMATION_LEVEL:
            numGraphPixels = _pixelsPerMeter - (_pixelsPerMeter / 3) - ((_pixelsPerMeter / 3) % 2);
            pointsPerPixel = 100/numGraphPixels;
            numLitPixels = meterLevel[meterNum] / pointsPerPixel;
            for ( int pixelNum = 0; pixelNum < _pixelsPerMeter; pixelNum++)
            {
              byte targetPixel = (pixelNum + (numGraphPixels/2)) % _pixelsPerMeter;

              if ( numLitPixels > targetPixel )
              {
                r = rgb[0];
                g = rgb[1];
                b = rgb[2];
              }
              else if (targetPixel >= numGraphPixels)
              {
                r = 200;
                g = 200;
                b = 240;
              }
              else
              {
                r = 0;
                g = 0;
                b = 0;
              }
              pixels.setPixelColor(((15 - (meterNum * _pixelsPerMeter + pixelNum))), r, g, b);
            }
            break;
          case ANIMATION_LEVEL2:
            numGraphPixels = _pixelsPerMeter - (_pixelsPerMeter / 3) - ((_pixelsPerMeter / 3) % 2);
            pointsPerPixel = 100/_pixelsPerMeter;
            numLitPixels = meterLevel[meterNum] / pointsPerPixel;
            for ( int pixelNum = 0; pixelNum < _pixelsPerMeter; pixelNum++)
            {
              byte targetPixel = (pixelNum + (_pixelsPerMeter/2)) % _pixelsPerMeter;

              if ( numLitPixels > targetPixel )
              {
                r = rgb[0];
                g = rgb[1];
                b = rgb[2];
              }
              else
              {
                r = 0;
                g = 0;
                b = 0;
              }
              pixels.setPixelColor(((15 - (meterNum * _pixelsPerMeter + pixelNum))), r, g, b);
            }
            break;
          case ANIMATION_POUR:
            a = (animationCounter[meterNum] >> 4) & 15;
            c =  animationCounter[meterNum]       & 15;
            for ( int pixelNum = 0; pixelNum < _pixelsPerMeter; pixelNum++)
            {
              d = (a + 1) & 15;
              r = max(rgb[0], (pgm_read_byte(&sine[a%8]) * (16 - c) + pgm_read_byte(&sine[d%8]) * c  ) >> 4);
              g = max(rgb[1], (pgm_read_byte(&sine[a%8]) * (16 - c) + pgm_read_byte(&sine[d%8]) * c  ) >> 4);
              b = max(rgb[2], (pgm_read_byte(&sine[a%8]) * (16 - c) + pgm_read_byte(&sine[d%8]) * c  ) >> 4);
              pixels.setPixelColor((((meterNum * _pixelsPerMeter + pixelNum))), r, g, b);
              a = d;
            }
            animationCounter[meterNum] -= 5;
            break;
        }
      }
    }
    lastDisplayMillis = nowTime;
    pixels.setBrightness(currentBrightness);
    pixels.show();
  }
}

void NeoPixelMeter::hsi2rgb(float H, float S, float I, int *rgb) {
  H = fmod(H,120);
  H = 3.14159*H/(float)180;
  S = S>0?(S<1?S:1):0;
  I = I>0?(I<1?I:1):0;

  rgb[0] = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
  rgb[1] = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
  rgb[2] = 255*I/3*(1-S);
}
