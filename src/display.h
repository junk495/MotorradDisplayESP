#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>

// Forward declaration for LCD wrapper class
class OldLcdWrapper {
  public:
	void setBrightness(uint8_t b);
	void setOffset(int x, int y);
};

// External declarations for display objects
extern TFT_eSPI tft;
extern OldLcdWrapper lcd;

#endif