#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Include the built-in GFX fonts
#include <Fonts/FreeMonoBoldOblique12pt7b.h>  // Font for Wi-Fi messages
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h> 

/*

https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts

Adafruit_GFX.h built-in fonts

FreeMono12pt7b.h		FreeSansBoldOblique12pt7b.h
FreeMono18pt7b.h		FreeSansBoldOblique18pt7b.h
FreeMono24pt7b.h		FreeSansBoldOblique24pt7b.h
FreeMono9pt7b.h			FreeSansBoldOblique9pt7b.h
FreeMonoBold12pt7b.h		FreeSansOblique12pt7b.h
FreeMonoBold18pt7b.h		FreeSansOblique18pt7b.h
FreeMonoBold24pt7b.h		FreeSansOblique24pt7b.h
FreeMonoBold9pt7b.h		FreeSansOblique9pt7b.h
FreeMonoBoldOblique12pt7b.h	FreeSerif12pt7b.h
FreeMonoBoldOblique18pt7b.h	FreeSerif18pt7b.h
FreeMonoBoldOblique24pt7b.h	FreeSerif24pt7b.h
FreeMonoBoldOblique9pt7b.h	FreeSerif9pt7b.h
FreeMonoOblique12pt7b.h		FreeSerifBold12pt7b.h
FreeMonoOblique18pt7b.h		FreeSerifBold18pt7b.h
FreeMonoOblique24pt7b.h		FreeSerifBold24pt7b.h
FreeMonoOblique9pt7b.h		FreeSerifBold9pt7b.h
FreeSans12pt7b.h		FreeSerifBoldItalic12pt7b.h
FreeSans18pt7b.h		FreeSerifBoldItalic18pt7b.h
FreeSans24pt7b.h		FreeSerifBoldItalic24pt7b.h
FreeSans9pt7b.h			FreeSerifBoldItalic9pt7b.h
FreeSansBold12pt7b.h		FreeSerifItalic12pt7b.h
FreeSansBold18pt7b.h		FreeSerifItalic18pt7b.h
FreeSansBold24pt7b.h		FreeSerifItalic24pt7b.h
FreeSansBold9pt7b.h		FreeSerifItalic9pt7b.h
*/
// Font for the clock

// Wi-Fi credentials
const char* ssid = "your_ssid";
const char* password = "your_password";

// Time setup
const long utcOffsetInSeconds = -3 * 3600; // GMT-3

// Pins for SPI communication
#define TFT_CS    15
#define TFT_DC    2
#define TFT_RST   5
#define TFT_MOSI  23
#define TFT_SCLK  18

// Create an instance of the ST7789 display
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  // Initialize the SPI display
  tft.init(240, 320); // Initialize the display with width and height
  tft.setRotation(3); // Set the rotation to PORTRAIT_INVERTED (3 for portrait inverted mode)
  tft.fillScreen(ST77XX_BLACK); // Clear the screen with black color

  // Set up the smaller font for Wi-Fi messages
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1); // Text size multiplier (1 means normal size for this font)

  // Connect to Wi-Fi
  tft.setCursor(10, 50);
  tft.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(10, 50);
    tft.println("Connecting to Wi-Fi...");
  }

  // Once connected, display the IP address
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 50);
  tft.println("Wi-Fi connected!");

  // Display the IP address
  tft.setCursor(10, 90);
  tft.println(WiFi.localIP());

  // Initialize NTP Client
  timeClient.begin();
}

void loop() {
  // Update the time from NTP server
  timeClient.update();

  // Get the formatted time
  String formattedTime = timeClient.getFormattedTime();

  // Clear the previous time displayed
  tft.fillRect(0, 120, 240, 100, ST77XX_BLACK); // Clear a rectangle area where the time is displayed

  // Set up the larger font for the clock
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextSize(1); // Larger text size multiplier

  // Display the current time in large letters
  tft.setCursor(10, 180); // Adjust the cursor to center the text
  tft.println(formattedTime);

  // Wait for 1 second before updating the time again
  delay(1000);
}

