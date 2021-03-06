NeoPixelMeter
=============

An Arduino library meant to power chained NeoPixels as meter gauges

* **Demo**: [Single meter demo](https://www.youtube.com/watch?v=uW5UT9FoLpU)
* **Sample Enclosure**: [Beer Tap Collar](https://tinkercad.com/things/5KOgV5w68OU)
* **Requires**: [Adafruit's NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel)

It was designed and tested with Adafruit's NeoPixel Ring - 16, though it should work with larger NeoPixel rings, sticks, and strips. The above housing was printed with translucent PLA to fit around the base of the taps on my keg fridge.

It's still a little rough, so please feel free to branch and contribute back!

For a full description of features, check out [NeoPixelMeter.h](NeoPixelMeter.h)

A peek at some of the available commands:

    //Set the level illumnited for each meter
    void setLevel(int meterNum, int percentFull);

    //Set a meters's animation directly.
    void setAnimation(int meterNum, int animation);

    //When awake, the meter should show the scrolling animation
    void showScrolling(int meterNum);

    //When awake, the meter should show the level as a gauge
    void showLevel(int meterNum);

    //Fade to new brightness
    void setBrightnessWithFade(int brightness);

    //Set new brightness
    void setBrightness(int brightness);

    //Fade the meter on.
    void displayWakeup(int secondsAwake);

    //Set displays off
    void displaySleep();
