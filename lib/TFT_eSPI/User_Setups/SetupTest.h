// USER DEFINED SETTINGS
// Set driver type, fonts to be loaded, pins used and SPI control method etc
// For Waveshare 2.8inch Touch Display Module for Raspberry Pi Pico, 262K Colors, 320×240, SPI SKU: 19804
// https://www.waveshare.com/pico-restouch-lcd-2.8.htm

#define ST7789_DRIVER // Full configuration option, define additional parameters below for this display
#define TFT_RGB_ORDER TFT_BGR // Colour order Blue-Green-Red
#define TFT_WIDTH 240 // ST7789 240 x 240 and 240 x 320
#define TFT_HEIGHT 320 // ST7789 240 x 320
#define TFT_INVERSION_ON
#define TFT_BL 13 // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH // Level to turn ON back-light (HIGH or LOW)
#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_CS 9 // Chip select control pin
#define TFT_DC 8 // Data Command control pin
#define TFT_RST 15 // Reset pin (could connect to RST pin)
#define TOUCH_CS 16
#define LOAD_GLCD // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2 // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4 // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6 // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7 // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8 // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
#define SMOOTH_FONT
#define TFT_SPI_PORT 1 // Set to 0 if SPI0 pins are used, or 1 if spi1 pins used
#define SPI_FREQUENCY 40000000

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY 2000000

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY 2500000

#define SUPPORT_TRANSACTIONS