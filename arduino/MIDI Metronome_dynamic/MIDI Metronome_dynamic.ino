#include <MIDI.h>
#include <Adafruit_NeoPixel.h>
#include "Ring.h"
#include "RingA.h"
#include "RingB.h"

#define PIN_NEO_PIXEL 4  // Arduino pin that connects to NeoPixel
#define NUM_PIXELS 24    // The number of LEDs (pixels) on NeoPixel
#define DELAY_INTERVAL 20

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
Ring *ring;
bool started = false;

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  ring = new RingB();
  MIDI.setHandleNoteOn(NoteOnHandler);
  MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
  pinMode(LED_BUILTIN, OUTPUT);
  NeoPixel.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  NeoPixel.setBrightness(50);
}

void loop() {
  NeoPixel.clear();

  // Read incoming messages
  MIDI.read();

  if (ring == NULL) {
    // There will be no data to draw
    return;
  }

  //Draw
  for (int i = 0; i < NUM_PIXELS; i++) {
    Pixel *p = ring->pixels[i];
    NeoPixel.setPixelColor(i, NeoPixel.Color(p->r, p->g, p->b));
  }

  NeoPixel.show();
}

void NoteOnHandler(byte channel, byte pitch, byte velocity) {
  if (pitch == 3) {
    ring->stop();
    started = false;
  }
  else if (pitch == 4) {
    if (!started) {
      ring->start();
      started = true;
    }
    ring->tick();
  }
}