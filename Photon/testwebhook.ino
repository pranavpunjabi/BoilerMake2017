// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1331/Adafruit_SSD1331.h"
#include "Adafruit_SSD1331/Adafruit_GFX.h"

#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF
Adafruit_SSD1331 testOLED(A2, A0, A1);
void drawTimeMenu(int hour, int mins, Adafruit_SSD1331 &test_OLED);
void drawWeatherMenu(Adafruit_SSD1331 &oledScreen, int temperature, int windSpeed, int precipitation);
int led = D7;  // The on-board LED
void setup() 
{
    pinMode(led, OUTPUT);
    testOLED.begin();
}
void loop()
{
    digitalWrite(led, HIGH);   // Turn ON the LED
    testOLED.fillScreen(0x0000);
    drawTimeMenu(12, 22, testOLED);
    //drawWeatherMenu(testOLED, 22, 18, 5);
    String temperature = String(random(60, 80));
    Particle.publish("temp", temperature, PRIVATE);
    Particle.publish("weather");
    Particle.publish("dateTime");
    Particle.publish("capitalOne");
    Particle.publish("stocks");
    
    delay(30000);               // Wait for 30 seconds
    digitalWrite(led, LOW);    // Turn OFF the LED
    delay(30000);               // Wait for 30 seconds
}
void drawTimeMenu(int hour, int mins, Adafruit_SSD1331 &test_OLED)
{
    // dimensions: 96 * 64
    //drawDigit(int digit, int sRow, int sCol, int eRow, int eCol)
    test_OLED.fillScreen(BLACK);
    test_OLED.setTextColor(RED, BLACK);
    test_OLED.setCursor(5, 20);
    test_OLED.setTextSize(3);
Ready. 
