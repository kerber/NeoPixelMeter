#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#ifdef __AVR
 #include <avr/pgmspace.h>
#else
 #ifndef PROGMEM
  #define PROGMEM
 #endif
#endif

static const int8_t PROGMEM sine[] = { // Brightness table
    0, 28, 96, 164, 192, 164, 96, 28 };

#endif // _ANIMATION_H_
