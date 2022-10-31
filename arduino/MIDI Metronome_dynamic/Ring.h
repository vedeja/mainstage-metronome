#include "Pixel.h"

class Ring {
private:
  bool isStarted = false;
  int count;

protected:
  virtual void startHandler() = 0;
  virtual void stopHandler() = 0;
  virtual void beatStartedHandler() = 0;
  virtual void beatEndedHandler() = 0;
  virtual void tickHandler(int count) = 0;

  void clear() {
    // Set all black
    for (int i = 0; i < 24; i++) {
      pixels[i]->set(0, 0, 0);
    }
  }

public:
  Pixel *pixels[24];
  Ring() {
    // Initiate pixels
    for (int i = 0; i < 24; i++) {
      pixels[i] = new Pixel();
    }
  }

  void start() {
    count = 0;
    isStarted = true;
    startHandler();
  }

  void stop() {
    isStarted = false;
    clear();
    stopHandler();
  }

  void tick() {
    if (!isStarted) {
      return;
    }

    if (count == 1) {
      beatStartedHandler();
    }

    // If beat is over
    if (count == 24) {
      beatEndedHandler();
      count = 0;
    }

    clear();
    tickHandler(count);
    count++;
  };
};