#include <FastLED.h>

/* NeoPixel Shield data pin is always 6. Change for other boards */
#define CONTROL_PIN 6

/* Board shape and size configuration. Sheild is 8x5, 40 pixels */
#define HEIGHT 8
#define WIDTH 8
#define NUM_LEDS HEIGHT*WIDTH

CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;

void setup() {
  FastLED.addLeds<NEOPIXEL, CONTROL_PIN>(leds, NUM_LEDS);
  
  /* Set a black-body radiation palette
     This comes from FastLED */
  gPal = HeatColors_p; 
  
  /* Clear display before starting */
  FastLED.clear();
  FastLED.show();
  FastLED.delay(1500); // Sanity before start

  Serial.begin(9600);
}

/* Refresh rate. Higher makes for flickerier
   Recommend small values for small displays */
#define FPS 120
#define FPS_DELAY 1000/FPS
#define HOT 180
#define MAXHOT HOT*HEIGHT

int ticks = 0;

void loop() {
  random16_add_entropy( random() ); // We chew a lot of entropy
  
  Fireplace();

  FastLED.show();
  FastLED.delay(FPS_DELAY); //
  ticks++;
}

int sparkChance = 1000;
int spreadChance = 100;
uint8_t maxHeat = 180;
uint8_t brightness = 255;
int stack[WIDTH][HEIGHT];        // stacks that are cooler

void Fireplace () {
  for (int x=0; x < WIDTH; x++) {
    for (int y=0; y < HEIGHT; y++) {     
      if (stack[x][y]>0)
        stack[x][y]--;

      if (random(0, spreadChance)) {
        true;
      }

      if (random(0,((sin(float(ticks)/1000.0f)+1) * sparkChance) + sparkChance/2) == 0) {
        stack[x][y] = random(0,(maxHeat));
      }
    }
  }

  Serial.println((sin(float(ticks)/100.0f)+1) * sparkChance);
  

  // 4. map stacks to led array
  for( int i = 0; i < WIDTH; i++) {
  for( int j = 0; j < HEIGHT; j++) {
     leds[(j*WIDTH) + i] = ColorFromPalette(gPal, stack[i][j], brightness, LINEARBLEND);
  }
  }
}

void copy(int* src, int* dst, int len) {
    for (int i = 0; i < len; i++) {
        *dst++ = *src++;
    }
}
