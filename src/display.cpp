/**
 * Display Implementation
 *
 * This file contains the actual definitions for the display-related global objects
 * to avoid multiple definition errors when headers are included in multiple translation units.
 */

#include "display.h"
#include "config.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

// Actual TFT display instance definition
TFT_eSPI tft = TFT_eSPI();

// LCD wrapper instance for backward compatibility
OldLcdWrapper lcd;

// Implementation of OldLcdWrapper methods
void OldLcdWrapper::setBrightness(uint8_t b) {
#if defined(PIN_BACKLIGHT) && (PIN_BACKLIGHT >= 0) && (PIN_BACKLIGHT <= 45) && !(PIN_BACKLIGHT == 0) && \
!(PIN_BACKLIGHT == 1) && !(PIN_BACKLIGHT == 3) && !(PIN_BACKLIGHT == 4) && !(PIN_BACKLIGHT == 5)
	pinMode(PIN_BACKLIGHT, OUTPUT);
	if (b == 0) {
		digitalWrite(PIN_BACKLIGHT, LOW);
	} else {
		digitalWrite(PIN_BACKLIGHT, HIGH);
	}
#else
	Serial.println("setBrightness: Backlight pin not valid – skipping");
#endif
}

void OldLcdWrapper::setOffset(int x, int y) {
	// Dummy - nicht benötigt für dieses Display
}