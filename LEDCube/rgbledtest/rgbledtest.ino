#include <SoftwareSerial.h>  
#include <Adafruit_NeoPixel.h>
#include <stdlib.h>

#define PIN 13
#define LED_COUNT 4

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3
char val = '0';
//String sum;
char sum [10];
int i = 0;
uint32_t color;

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup(){

  leds.begin();  // Call this to start up the LED strip.
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.show();   // ...but the LEDs don't actually update until you call this.
}

void loop(){

  leds.setPixelColor(0,0x00FF00);
  leds.show();
}

// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}

