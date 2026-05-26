/**
 * Display Implementation
 *
 * This file contains the actual definitions for the display-related global objects
 * to avoid multiple definition errors when headers are included in multiple translation units.
 */

#include "display.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

// Actual TFT display instance definition
TFT_eSPI tft = TFT_eSPI();

// LCD wrapper instance for backward compatibility
OldLcdWrapper lcd;

// Implementation of OldLcdWrapper methods
void OldLcdWrapper::setBrightness(uint8_t b) {
	// TFT_eSPI verwendet digitalWrite für Backlight
	// Pin 1 ist typisch für ESP32-S3 Display Boards
	pinMode(1, OUTPUT);
	if (b == 0) {
		digitalWrite(1, LOW);
	} else {
		digitalWrite(1, HIGH);
		// Für PWM-Helligkeit müsstest du ledcWrite verwenden
		// Aber ST7789 hat oft nur On/Off
	}
}

void OldLcdWrapper::setOffset(int x, int y) {
	// Dummy - nicht benötigt für dieses Display
}