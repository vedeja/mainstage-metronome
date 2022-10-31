class RingA : public Ring {
public:
  RingA()
    : Ring() {
  }

private:
  int direction;
  bool flashIsOn = false;

  virtual void startHandler() {
    direction = 1;
  }

  virtual void stopHandler() {
  }

  virtual void tickHandler(int count) {
    if (flashIsOn) {
      // Flash
      int brightness = 8;
      for (int i = 0; i < 24; i++) {
        pixels[i]->set(brightness, brightness, brightness);
      }

      // Turn off flash after one tick
      if (count > 1) {
        flashIsOn = false;
      }
    }

    drawPendulum(count);
  }

  virtual void beatStartedHandler() {
    flashIsOn = true;
  }

  virtual void beatEndedHandler() {
    direction *= -1;
  }

  void drawPendulum(int count) {
    // Map from 0-23 to 0-17
    int index = round(map(count, 0, 23, 0, 17));

    // If moving backwards, calculate from the other end
    if (direction == -1) {
      index = 17 - index;
    }

    // Offset
    index += 3;

    // Draw
    pixels[index]->set(255, 0, 0);
  }
};