/*
 * Bluetooth LED code
 * 
 * Communicates via Bluetooth to WS2812 RGB leds
 * 
 */
 
#include <SoftwareSerial.h>  
#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"
#include <stdlib.h>
#include <string.h>

#define PIN 13
#define LED_COUNT 4
#define BUFF_LEN 1024
#define STEPS 50
#define DELAY 25

int bluetoothTx = 3;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 2;  // RX-I pin of bluetooth mate, Arduino D3
char val = '0';
//String sum;
char code[BUFF_LEN];
int i = 0;
uint32_t color = 0;
uint32_t n_color = 0;

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{

  int x;
  for(x = 0; x < BUFF_LEN; x++){
    code[x] = 0;
  }
  
  Serial.begin(9600);  // Begin the serial monitor at 9600bps

  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600

  leds.begin();  // Call this to start up the LED strip.

  startup_display(&color);
  delay(100);
  //clearLEDs();   // This function, defined below, turns all LEDs off...
  //leds.show();   // ...but the LEDs don't actually update until you call this.
}

void loop()
{
  while(i < BUFF_LEN){
    if(bluetooth.available())  // If the bluetooth sent any characters
    {
      // Send any characters the bluetooth prints to the serial monitor
      val = (char)bluetooth.read();
      Serial.print("Received: ");
      Serial.println(val);
      delay(50);
      if(val == '\n')
      {
        break;
      }
      code[i++] = val;
    }
  }
  n_color = strtoul(code, NULL, 0);
  Serial.print("Received: ");
  Serial.println(code);
  Serial.print("Old Color Val: ");
  Serial.println(color);
  Serial.print("New Color Val: ");
  Serial.println(n_color);
  transition(&color, n_color);
  
  for(i = 0; i < BUFF_LEN; i++)
    code[i] = 0;
  
  i = 0;
}

void startup_display(uint32_t *color) {
  (*color) = 0;
  uint32_t off = 0x0;
  transition(color, 0xFFFFFF);
  delay(250);
  transition(color, off);
}

void transition(uint32_t* old, uint32_t n) {
  //Pull out RGB values for old and new
  uint32_t oldb = (*old & 0x0FF);
  uint32_t nb = (n & 0x0FF);
  uint32_t oldg = ((*old)>>8 & 0x0FF);
  uint32_t ng = (n>>8 & 0x0FF);
  uint32_t oldr = ((*old)>>16 & 0x0FF);
  uint32_t nr = (n>>16 & 0x0FF);
  Serial.println("Old color");
  printRGB(oldr, oldg,oldb);
  Serial.println("New color");
  printRGB(nr, ng,nb);
  
  // Calculate the step for each field.
  float rStep = getStep(oldr, nr);
  float gStep = getStep(oldg, ng);
  float bStep = getStep(oldb, nb);

  int i; //Preform transition
  for(i = 0; i < STEPS; i++) {
    oldb += bStep;
    oldg += gStep;
    oldr += rStep;
    correct(&oldb);
    correct(&oldg);
    correct(&oldr);
    
    uint32_t c = 0;
    c |= oldb;
    c |= (oldg << 8);
    c |= (oldr << 16);
    setColor(c);
  }
  setColor(n);
  *old = n; // Set old to be the new color.
}

float getStep(int old, int n) {
  float step = (n - (float)old)/STEPS;
  return step;
}

void setColor(uint32_t c) {
      int n;
      for(n = 0; n < LED_COUNT; n++)
        leds.setPixelColor(n, c);
      leds.show();
      delay(DELAY);
}

void printRGB(uint32_t r, uint32_t g, uint32_t b) {
  Serial.print("R: ");
  Serial.println(r);
  Serial.print("G: ");
  Serial.println(g);
  Serial.print("B: ");
  Serial.println(b);
}

void correct(uint32_t* val) {
  if(*val > 255)
    *val = 0;
}


