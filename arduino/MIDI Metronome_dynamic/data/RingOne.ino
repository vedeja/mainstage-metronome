#include <MIDI.h>
#include <Adafruit_NeoPixel.h>

#define PIN_NEO_PIXEL 4  // Arduino pin that connects to NeoPixel
#define NUM_PIXELS 24    // The number of LEDs (pixels) on NeoPixel
#define DELAY_INTERVAL 20

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

class Pixel {
public:
  int r;
  int g;
  int b;
};

class Ring1 {
private:
  int count = 0;
  int direction = 1;
  int currentIndex = 0;
  bool flashIsOn = false;

public:
  Pixel *pixels[24];
  Ring1() {
    // Initiate pixels
    for (int i = 0; i < 24; i++) {
      pixels[i] = new Pixel();
    }
  }

  void start() {
    count = 0;
    currentIndex = 12;

    // Set all black
    for (int i = 0; i < 24; i++) {
      Pixel *px = pixels[i];
      px->r = 0;
      px->g = 0;
      px->b = 0;
    }
  }

  void stop() {
  }

  void tick() {
    count++;

    if (count == 24) {
      direction *= -1;
      count = 0;
      flashIsOn = true;
    }

    // Clear
    for (int i = 0; i < 24; i++) {
      pixels[i]->r = 0;
      pixels[i]->g = 0;
      pixels[i]->b = 0;
    }

    if (flashIsOn) {
      for (int i = 0; i < 24; i++) {
        pixels[i]->r = 50;
        pixels[i]->g = 50;
        pixels[i]->b = 50;
      }

      if (count > 1) {
        flashIsOn = false;
      }
    }

    int c = round(map(count, 0, 23, 0, 17));

    if (currentIndex != c) {
      currentIndex = c;
    }

    if (direction == -1) {
      c = 17 - c;
    }

    c += 3;

    for (int i = 0; i < 24; i++) {
      if (i == c) {
        pixels[i]->r = 255;
        pixels[i]->g = 0;
        pixels[i]->b = 0;
        if (c == 9 + 3) {
          pixels[i]->r = 0;
          pixels[i]->g = 255;
          pixels[i]->b = 0;
        }
      }
    }
  }
};

const int buttonPin = 2;
int buttonState = 0;
int clockCount = 0;
bool buttonWasReleased = false;
int currentPixel = 4;
int direction = 1;
Ring1 *r1;

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  r1 = new Ring1();
  MIDI.setHandleClock(ClockHandler);
  MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
  pinMode(LED_BUILTIN, OUTPUT);
  NeoPixel.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  NeoPixel.setBrightness(50);

  // Button
  pinMode(buttonPin, INPUT);

  r1->start();
}

void loop() {
  NeoPixel.clear();

  advance();
  delay(40);

  // Send note 42 with velocity 127 on channel 1
  //MIDI.sendNoteOn(42, 127, 1);

  // Read incoming messages
  //MIDI.read();

  buttonState = digitalRead(buttonPin);  // wait for a second

  if (buttonState == HIGH && buttonWasReleased == true) {
    //clockCount++;
    buttonWasReleased = false;
    //Serial.print(clockCount);
    //Serial.println();
    advance();
  } else if (buttonState == LOW) {
    buttonWasReleased = true;
  }

  // if (clockCount == 4) {
  //   // digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  //   // delay(200);                       // wait for a second
  //   // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  //   advance();
  //   clockCount = 0;
  // }


  // Draw
  for (int i = 0; i < NUM_PIXELS; i++) {
    Pixel *p = r1->pixels[i];
    NeoPixel.setPixelColor(i, NeoPixel.Color(p->r, p->g, p->b));
  }

  NeoPixel.show();
}

void ClockHandler() {
}

void advance() {
  r1->tick();
}