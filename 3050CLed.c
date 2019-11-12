#pragma systemFile	// eliminate warning for "unreferenced" functions

const int NUM_LED = 18;

#define LOW 0
#define HIGH 1
#define uint32_t unsigned long
#define uint16_t unsigned int
#define uint8_t ubyte

task ledDemoMode();
task ledTask();

void setupLedStrip();
void allocBuffer();
void startBitBang();
void disableLEDs();
void enableLEDs();
void clearLEDs();
void setLEDTimerOverride(bool state);
void updateLedStrip(void);
uint32_t RGB2GRB(byte r, byte g, byte b);
void setPixelColor(uint16_t n, uint32_t c);
void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
void colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait);
void colorWipe(uint32_t grb, uint8_t wait);
void colorChase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait);
void colorChase(uint32_t grb, uint8_t wait);
void theaterChase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait, uint16_t duration);
void theaterChaseTask(uint8_t r, uint8_t g, uint8_t b, uint8_t wait, uint16_t duration);
void theaterChaseRainbow(uint8_t wait);
void rainbowFade(uint8_t wait);
void rainbowCycle(uint8_t wait, uint16_t duration);
void rainbowCycleTask(uint8_t wait, uint16_t duration);
uint32_t colorWheel(uint16_t WheelPos);

const uint16_t numBytes = (NUM_LED * 3) + ((NUM_LED + 31) / 32);   // Size of 'pixels' buffer below
uint8_t pixels[numBytes];  // Holds LED color values (3 bytes each) + latch bytes
bool LEDS_ENABLED = true;

byte ledMode;
uint8_t ledColorR;
uint8_t ledColorG;
uint8_t ledColorB;
uint8_t ledWait;
uint16_t ledDuration;
bool ledTimerOverride;
bool ledTaskRunning = false;

void setupLedStrip() {
	allocBuffer();
	startBitBang();

	// Update the strip with all LEDs off
	updateLedStrip();
}

// Lay out all the data in one big flat buffer incl pixel color bytes, latch data, etc.
void allocBuffer() {
	uint16_t dataBytes  = NUM_LED * 3;
	uint8_t latchBytes = (NUM_LED + 31) / 32;
	memset( pixels           , 0x80, dataBytes);  // Init to RGB 'off' state
	memset(&pixels[dataBytes], 0   , latchBytes); // Clear latch bytes
}

// Enable software SPI pins and issue initial latch
void startBitBang() {
	SensorValue[MOSI] = LOW;

	for (uint16_t i = ((NUM_LED + 31) / 32) * 8; i > 0; i--) {
		SensorValue[SCLK] = HIGH;
		SensorValue[SCLK] = LOW;
	}
}

void disableLEDs()
{
	LEDS_ENABLED = false;

	// Turn off all LEDs
	for (int i = 0; i < NUM_LED; i++) setPixelColor(i, 0);

	uint8_t  *ptr = pixels;
	uint16_t i    = numBytes;

	uint8_t p;
	while (i--) {
		p = *ptr++;
		for (uint8_t bit = 0x80; bit; bit >>= 1) {
		SensorValue[MOSI] = (p & bit) ? HIGH : LOW;
			SensorValue[SCLK] = HIGH;
			SensorValue[SCLK] = LOW;
		}
	}
}

void enableLEDs()
{
	LEDS_ENABLED = true;
}

void clearLEDs()
{
	// Turn off all LEDs
	for (int i = 0; i < NUM_LED; i++) setPixelColor(i, 0);
	updateLedStrip();
}

void setLEDTimerOverride(bool state)
{
	ledTimerOverride = state;
}

// Light up LED strip using data in the pixels buffer
void updateLedStrip(void) {
	if (!LEDS_ENABLED) return;

	uint8_t  *ptr = pixels;
	uint16_t i    = numBytes;

	uint8_t p;
	while (i--) {
		p = *ptr++;
		for (uint8_t bit = 0x80; bit; bit >>= 1) {
		SensorValue[MOSI] = (p & bit) ? HIGH : LOW;
			SensorValue[SCLK] = HIGH;
			SensorValue[SCLK] = LOW;
		}
	}
}

// Convert separate R,G,B into combined 32-bit GRB color
uint32_t RGB2GRB(byte r, byte g, byte b) {
	return ((uint32_t)(g | 0x80) << 16) | ((uint32_t)(r | 0x80) <<  8) | b | 0x80 ;
}

// Set pixel color from separate 7-bit R, G, B components
void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
	if (!LEDS_ENABLED) return;

	if (n < NUM_LED) {
		uint8_t *p = &pixels[n * 3];
		*p++ = g | 0x80; // Strip color order is GRB,
		*p++ = r | 0x80; // not the more common RGB,
		*p++ = b | 0x80; // so the order here is intentional; don't "fix"
	}
}

// Set pixel color from 'packed' 32-bit GRB (not RGB) value
void setPixelColor(uint16_t n, uint32_t c) {
	if (!LEDS_ENABLED) return;

	if (n < NUM_LED) {
		uint8_t *p = &pixels[n * 3];
		*p++ = (c >> 16) | 0x80;
		*p++ = (c >>  8) | 0x80;
		*p++ =  c        | 0x80;
	}
}

// Fill the dots progressively along the strip
void colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
	if (!LEDS_ENABLED) return;

	colorWipe(RGB2GRB(r, g, b), wait);
}

void colorWipe(uint32_t grb, uint8_t wait) {
	if (!LEDS_ENABLED) return;

	for (int i = 0; i < NUM_LED; i++) {
		setPixelColor(i, grb);
		updateLedStrip();          // Update LED states
		delay(wait);
	}
	clearLEDs();
}

// Chase one dot down the full LED strip
void colorChase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
	if (!LEDS_ENABLED) return;

	colorChase(RGB2GRB(r, g, b), wait);
}
void colorChase(uint32_t grb, uint8_t wait) {
	if (!LEDS_ENABLED) return;

	// Start by turning all pixels off
	for (int i = 0; i < NUM_LED; i++) setPixelColor(i, 0);

	// Then display one pixel at a time
	for (int i = 0; i < NUM_LED; i++) {
		setPixelColor(i, grb); // Turn on pixel
		updateLedStrip();          // Update LED states
		setPixelColor(i, 0);       // Erase pixel, but don't refresh!
		delay(wait);
	}

	updateLedStrip();  // Update to turn off last pixel
}

void theaterChase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait, uint16_t duration) {
	if (!LEDS_ENABLED) return;
	static unsigned long ledExpirationTime = 0;
	ledExpirationTime = nSysTime + duration;

	while (ledExpirationTime > nSysTime || ledTimerOverride) {
		for (int q = 0; q < 3; q++) {
			for (int i = 0; i < NUM_LED; i = i + 3) {
				if (ledExpirationTime <= nSysTime && !ledTimerOverride) goto theaterChaseDone;
				setPixelColor(i + q, r, b, g); // Turn on every third pixel
			}
			updateLedStrip();          // Update LED states

			delay(wait);

			for (int i = 0; i < NUM_LED; i = i + 3) {
				if (ledExpirationTime <= nSysTime && !ledTimerOverride) goto theaterChaseDone;
				setPixelColor(i + q, 0); // Turn off every third pixel
			}
		}
	}
theaterChaseDone:
	clearLEDs();
}

void theaterChaseTask(uint8_t r, uint8_t g, uint8_t b, uint8_t wait, uint16_t duration) {
	if (!ledTaskRunning) {
		ledMode = 1;
		ledColorR = r;
		ledColorG = g;
		ledColorB = b;
		ledWait = wait;
		ledDuration = duration;
		startTask(ledTask);
	}
}

// Theater-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
	if (!LEDS_ENABLED) return;

	for (int j = 0; j < 384; j += 6) {     // Cycle through colors in the color wheel
		for (int q = 0; q < 3; q++) {
			for (int i = 0; i < NUM_LED; i = i + 3) {
				setPixelColor(i + q, colorWheel((i + j) % 384));    // Turn every third pixel on
			}
			updateLedStrip();          // Update LED states

			delay(wait);

			for (int i = 0; i < NUM_LED; i = i + 3) {
				setPixelColor(i + q, 0); // Turn off every third pixel
			}
		}
	}
	clearLEDs();
}

void rainbowFade(uint8_t wait) {
	if (!LEDS_ENABLED) return;

	for (int j = 0; j < 384; j++) {     // Cycle through colors in the color wheel
		for (int i = 0; i < NUM_LED; i++) {
			setPixelColor(i, colorWheel((i + j) % 384));
		}
		updateLedStrip();          // Update LED states
		delay(wait);
	}
	clearLEDs();
}

// Equally distributed the rainbow wheel along the LED strip
void rainbowCycle(uint8_t wait, uint16_t duration) {
	if (!LEDS_ENABLED) return;
	static unsigned long ledExpirationTime = 0;
	ledExpirationTime = nSysTime + duration;

	while (ledExpirationTime > nSysTime || ledTimerOverride) {
		for (int j = 383; j >= 0; j -= 10) {     // Cycle through colors in the color wheel
			for (int i = 0; i < NUM_LED; i++) {
				if (ledExpirationTime <= nSysTime && !ledTimerOverride) goto rainbowCycleDone;
				// Use each pixel as a fraction of the full 384-color wheel (thats the i / NUM_LED part)
				// Add in j which makes the colors go around per pixel
				// The % 384 is to make the wheel cycle around
				setPixelColor(i, colorWheel(((i * 384 / NUM_LED) + j) % 384));
			}
			updateLedStrip();          // Update LED states
			//delay(wait);
		}
	}
rainbowCycleDone:
	clearLEDs();
}

void rainbowCycleTask(uint8_t wait, uint16_t duration) {
	if (!ledTaskRunning) {
		ledMode = 2;
		ledColorR = 0;
		ledColorG = 0;
		ledColorB = 0;
		ledWait = wait;
		ledDuration = duration;
		startTask(ledTask);
	}
}

// Takes a value 0 to 384 and returns a color value
// The colors are a transition rgb back to r
uint32_t colorWheel(uint16_t WheelPos)
{
	byte r, g, b;
	switch(WheelPos / 128) {
	case 0:
		r = 127 - WheelPos % 128;   // Red down
		g = WheelPos % 128;         // Green up
		b = 0;                      // Blue off
		break;
	case 1:
		g = 127 - WheelPos % 128;   // Green down
		b = WheelPos % 128;         // Blue up
		r = 0;                      // Red off
		break;
	case 2:
		b = 127 - WheelPos % 128;   // Blue down
		r = WheelPos % 128;         // Red up
		g = 0;                      // Green off
		break;
	}
	return(RGB2GRB(r, g, b));
}

// Start selected LED strip patterns in a separate execution thread
task ledTask() {
	ledTaskRunning = true;

	//writeDebugStreamLine("mode: %d, r: %d, g: %d, b: %d, wait: %d, duration: %d", ledMode, ledColorR, ledColorG, ledColorB, ledWait, ledDuration);
	switch (ledMode) {
	case 1:
		theaterChase(ledColorR, ledColorG, ledColorB, ledWait, ledDuration);
		break;
	case 2:
		rainbowCycle(ledWait, ledDuration);
		break;
	}

	ledTaskRunning = false;
}

task ledDemoMode()
{
	while (1) {
		// Color wheel progression: Red, Pink, Violet, White?, Blue, Cyan, Green, Yellow, Orange

		colorWipe(127,   0,   0, 10); // Red
		colorWipe(127, 127,   0, 10); // Violet
		colorWipe(127,   0, 127, 10); // Yellow
		colorWipe(  0,   0, 127, 10); // Green
		colorWipe(  0, 127, 127, 10); // Cyan
		colorWipe(  0, 127,   0, 10); // Blue
		//colorWipe(127, 127, 127, 10); // White

  	colorChase(127,   0,   0, 20); // Red
	  colorChase(127, 127,   0, 20); // Violet
	  colorChase(127, 127, 127, 20); // White
	  colorChase(  0, 127,   0, 20); // Blue
	  colorChase(  0, 127, 127, 20); // Cyan
	  colorChase(  0,   0, 127, 20); // Green
	  colorChase(127,   0, 127, 20); // Yellow

	  theaterChase(127,   0,   0, 50, 1000); // Red
	  theaterChase(127, 127,   0, 50, 1000); // Violet
	  theaterChase(127, 127, 127, 50, 1000); // White
	  theaterChase(  0, 127,   0, 50, 1000); // Blue
	  theaterChase(  0, 127, 127, 50, 1000); // Cyan
	  theaterChase(  0,   0, 127, 50, 1000); // Green
	  theaterChase(127,   0, 127, 50, 1000); // Yellow

	  theaterChaseRainbow(10);

		rainbowFade(0);

		for (int i = 0; i < 8; i++) rainbowCycle(0, 1000);
	}
}
