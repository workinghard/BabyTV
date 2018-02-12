// http://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/
// https://www.arduino.cc/en/tutorial/potentiometer
// http://www.instructables.com/id/How-to-use-Potentiometer-Arduino-Tutorial/

// http://www.digole.com/tools/PicturetoC_Hex_converter.php
// Code format: DEC
// Display position: X0/Y0
// Used for: 262K Color(3bytes/pixel:XXXXXXXX00)

#include "FastLED.h"
#include <RotaryEncoder.h>

FASTLED_USING_NAMESPACE

#define WAITFORTICKS 5000

#define DATA_PIN    11
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    256
CRGB leds[NUM_LEDS];

#define BUTTON_PIN 12
int buttonReading = 0;           // the current value read from the input pin
int buttonCurrent_state = LOW;    // the debounced input value
long buttonTime = 0;         // the last time the output pin was sampled
int buttonCounter = 0;       // how many times we have seen new value
int buttonDebounce_count = 10; // number of millis/samples to consider before declaring a debounced input
int buttonState = 1;       // 0 = display off , 1 = display on , default on
 
int currBrightness = 50;
//#define BRIGHTNESS         50  // Inital brightness default value, will be changed later
#define FRAMES_PER_SECOND  120

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

const uint8_t kMatrixWidth  = 16;
const uint8_t kMatrixHeight = 16;
const uint8_t kMatrixSerpentineLayout = true;

void matrix_code();
void rainbowWithGlitter();
void confetti();

// Mario animation
#include "imgMario1.c" // Mario1
const byte *gMarioFrames[3] = { mario1, mario2, mario3 };
void showMario1();
// Mushroom animation
#include "imgMushroom1.c" // Mushroom1
const byte *gMushroomFrames[3] = { mushroom1, mushroom2 };
void showMushroom1();
// Bee animation
#include "imgBee1.c"  // Bee1
const byte *gBeeFrames[3] = { bee1, bee2 };
void showBee1();
// Bat animation
#include "imgBat1.c"  // Bee1
const byte *gBatFrames[3] = { bat1, bat2 };
void showBat1();
// Frog animation
#include "imgFrog1.c"  // Bee1
const byte *gFrogFrames[3] = { frog1, frog2, frog3 };
void showFrog1();

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { showMario1, showMushroom1, showBee1, showBat1, showFrog1, matrix_code, rainbowWithGlitter, confetti };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

int tmpIndexI = 0;
int tmpIndexJ = 0;

#define POTPIN A0
int potValue=0; //save analog value

// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);

int gCurrentFrameNumber = 0;

#include <IRremote.h>

int RECV_PIN = 10;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(currBrightness);

  Serial.begin (9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(POTPIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {

  checkRotary();

  checkButton();

  EVERY_N_MILLISECONDS( 100 ) { // 100 ms for stable reading
    if ( buttonState > 0 ) { // only if display is on
      checkBrightness();
    }
  }  

  EVERY_N_MILLISECONDS( 100 ) {
   if (irrecv.decode(&results)) {
     if ( results.value != 4294967295 ) {
       //Serial.println(results.value, HEX);
       nextPattern();
     }
     irrecv.resume(); // Receive the next value
    }
  }
  
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  FastLED.show();
    // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}

void checkButton() {
  if(millis() != buttonTime) {
    buttonReading = digitalRead(BUTTON_PIN);

    if( buttonReading == buttonCurrent_state && buttonCounter > 0) {
      buttonCounter--;
    }
    if(buttonReading != buttonCurrent_state) {
       buttonCounter++; 
    }
    // If the Input has shown the same value for long enough let's switch it
    if(buttonCounter >= buttonDebounce_count) {
      buttonCounter = 0;
      buttonCurrent_state = buttonReading;
      if ( buttonCurrent_state == HIGH ) {
        // button pressed
        if ( buttonState == 1 ) { // Display is on, let's turn it off
          buttonState = 0;
          currBrightness = 0;
        }else{  // Display is off, let's turn it on
          buttonState = 1;
        }
      }else{
        // button released
      }
    }
    buttonTime = millis();
  }
}

void checkBrightness() {
  int diff = 0;
  potValue = analogRead(POTPIN);
  potValue = map(potValue, 0, 1023, 0, 150); // Map the value to the brightness
  if ( potValue > currBrightness ) {
    diff = potValue - currBrightness;
  }else{
    diff = currBrightness - potValue;
  }
  if ( diff > 2 ) { // Change Brightness in 2er steps
    FastLED.setBrightness(potValue);
    currBrightness = potValue;
  }
}

void nextFrame(int count) {
  // add one to the current pattern number, and wrap around at the end
  gCurrentFrameNumber = (gCurrentFrameNumber + 1) % count;
}

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  gCurrentFrameNumber = 0; // reset the frame counter
  //Serial.println("Next pattern");
}
void lastPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber - 1) % ARRAY_SIZE( gPatterns);
  gCurrentFrameNumber = 0; // reset the frame counter
  //Serial.println("Last pattern");
}

void checkRotary() {
  for (tmpIndexI=0;tmpIndexI<WAITFORTICKS;tmpIndexI++) {
    static int pos = 0;
    encoder.tick();
  
    int newPos = encoder.getPosition();
    if (pos != newPos) {
      //Serial.print(newPos);
      //Serial.println();
      if ( newPos > pos ) {
        nextPattern();
      }else{
        lastPattern();
      }
      pos = newPos;
    } // if
  }
}

// convert bitmap to x/y coordinates
uint16_t getImgX(uint16_t index) {
  return index % kMatrixWidth;
}
uint16_t getImgY(uint16_t index) {
  return (kMatrixWidth - (index / kMatrixWidth) - 1);
}

// convert x/y cordinates to LED index on zig-zag grid
uint16_t getIndex(uint16_t x, uint16_t y) {
  uint16_t index;
  if (y == 0) {
    index = x;
  } else if (y % 2 == 0) {
    index = y * kMatrixWidth + x;
  } else {
    index = ((y * kMatrixWidth) + (kMatrixWidth-1)) - x;
  }
  return index;
}

// Confetti (OK)
void confetti()  {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

// Rainbow with Glitter (OK)
void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}
void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}
void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

// Matrix code (OK)
void matrix_code() { 
  EVERY_N_MILLIS(100) // falling speed
  {
    // move code downward
    // start with lowest row to allow proper overlapping on each column
    for (int8_t row=0; row<kMatrixWidth; row++) {
      for (int8_t col=0; col<kMatrixHeight; col++) {
        if (leds[getIndex(row, col)] == CRGB(0,255,25)) {
          leds[getIndex(row, col)] = CRGB(27,130,29); // create trail
          if (col > 0) {
            leds[getIndex(row, col-1)] = CRGB(0,255,25);
          }
        }
      }
    }

    // fade all leds
    for(tmpIndexI = 0; tmpIndexI < NUM_LEDS; tmpIndexI++) {
      if (leds[tmpIndexI].g != 255) leds[tmpIndexI].nscale8(192); // only fade trail
    }

    // check for empty screen to ensure code spawn
    bool emptyScreen = true;
    for(tmpIndexI=0; tmpIndexI < NUM_LEDS; tmpIndexI++) {
      if (leds[tmpIndexI]) {
        emptyScreen = false;
        break;
      }
    }

    // spawn new falling code
    if (random8(3) == 0 || emptyScreen) { // lower number == more frequent spawns 
      int8_t spawnX = random8(kMatrixWidth);
      leds[getIndex(spawnX, kMatrixHeight-1)] = CRGB(0,255,25 );
    }

    //FastLED.show();
  }
}

void showMario1() {
  tmpIndexJ = 0;
  EVERY_N_MILLISECONDS( 200 ) {  // 200ms frame change
    for (tmpIndexI=0;tmpIndexI<768;tmpIndexI=tmpIndexI+3) {
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].r = pgm_read_byte_near(gMarioFrames[gCurrentFrameNumber] + tmpIndexI);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].g = pgm_read_byte_near(gMarioFrames[gCurrentFrameNumber] + tmpIndexI+1);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].b = pgm_read_byte_near(gMarioFrames[gCurrentFrameNumber] + tmpIndexI+2);      
      tmpIndexJ=tmpIndexJ+1;
    }
    nextFrame(3);
  }
}


void showMushroom1() {
  tmpIndexJ = 0;
  EVERY_N_MILLISECONDS( 200 ) {  // 200ms frame change
    for (tmpIndexI=0;tmpIndexI<768;tmpIndexI=tmpIndexI+3) {
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].r = pgm_read_byte_near(gMushroomFrames[gCurrentFrameNumber] + tmpIndexI);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].g = pgm_read_byte_near(gMushroomFrames[gCurrentFrameNumber] + tmpIndexI+1);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].b = pgm_read_byte_near(gMushroomFrames[gCurrentFrameNumber] + tmpIndexI+2);      
      tmpIndexJ=tmpIndexJ+1;
    }
    nextFrame(2);
  }
}

void showBee1() {
  tmpIndexJ = 0;
  EVERY_N_MILLISECONDS( 200 ) {  // 200ms frame change
    for (tmpIndexI=0;tmpIndexI<768;tmpIndexI=tmpIndexI+3) {
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].r = pgm_read_byte_near(gBeeFrames[gCurrentFrameNumber] + tmpIndexI);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].g = pgm_read_byte_near(gBeeFrames[gCurrentFrameNumber] + tmpIndexI+1);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].b = pgm_read_byte_near(gBeeFrames[gCurrentFrameNumber] + tmpIndexI+2);      
      tmpIndexJ=tmpIndexJ+1;
    }
    nextFrame(2);
  }
}

void showBat1() {
  tmpIndexJ = 0;
  EVERY_N_MILLISECONDS( 200 ) {  // 200ms frame change
    for (tmpIndexI=0;tmpIndexI<768;tmpIndexI=tmpIndexI+3) {
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].r = pgm_read_byte_near(gBatFrames[gCurrentFrameNumber] + tmpIndexI);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].g = pgm_read_byte_near(gBatFrames[gCurrentFrameNumber] + tmpIndexI+1);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].b = pgm_read_byte_near(gBatFrames[gCurrentFrameNumber] + tmpIndexI+2);      
      tmpIndexJ=tmpIndexJ+1;
    }
    nextFrame(2);
  }
}

void showFrog1() {
  tmpIndexJ = 0;
  EVERY_N_MILLISECONDS( 200 ) {  // 200ms frame change
    for (tmpIndexI=0;tmpIndexI<768;tmpIndexI=tmpIndexI+3) {
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].r = pgm_read_byte_near(gFrogFrames[gCurrentFrameNumber] + tmpIndexI);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].g = pgm_read_byte_near(gFrogFrames[gCurrentFrameNumber] + tmpIndexI+1);
      leds[getIndex(getImgX(tmpIndexJ),getImgY(tmpIndexJ))].b = pgm_read_byte_near(gFrogFrames[gCurrentFrameNumber] + tmpIndexI+2);      
      tmpIndexJ=tmpIndexJ+1;
    }
    nextFrame(3);
  }
}
