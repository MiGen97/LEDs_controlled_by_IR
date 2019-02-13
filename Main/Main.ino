#include <IRremote.h>
#include <FastLED.h>

#define LED_PIN     7
#define NUM_LEDS    50
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER RBG
#define UPDATES_PER_SECOND 40

int RECV_PIN=6;
IRrecv irrecv(RECV_PIN);
CRGB leds[NUM_LEDS];

decode_results results;
bool changeFlag=false;  //flag for marking if is needed to change the LEDs at every loop
                        //set it false when the colour is static
                        //set it true when the LEDs are changing at very loop
const TProgmemPalette16 SeaColours_p PROGMEM =
{
    CRGB::DarkBlue,
    CRGB::DarkBlue,
    CRGB::DarkBlue,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Aqua,
    CRGB::Aqua,
    CRGB::Aqua,
    CRGB::Aqua,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::DarkBlue,
    CRGB::DarkBlue,
    CRGB::DarkBlue
};

void setup() {
  // put your setup code here, to run once:
   irrecv.enableIRIn(); // Start the receiver
   delay( 3000 ); // power-up safety delay
   FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
   FastLED.setBrightness( BRIGHTNESS );
}

void loop() {
  // put your main code here, to run repeatedly:
  static uint8_t startIndex = 0;
  if (irrecv.decode(&results)) {
    switch (results.value){
      //make the LEDs Red when the 0 button is pressed
      case 0x807F9A65:{
        changeFlag=false;
        break;
      }
      case 0x807F4AB5:{
        changeFlag=true;
        break;
      }
    }
    irrecv.resume(); // Receive the next value
  }
  if( changeFlag ){
    startIndex = startIndex + 1;
    FillLEDsFromPaletteColors( startIndex );
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }else{
    for( int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB(255,0,0);
        }
    FastLED.show();
    delay(100);
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( SeaColours_p, colorIndex, BRIGHTNESS, LINEARBLEND);
        colorIndex += 3;
    }
}
