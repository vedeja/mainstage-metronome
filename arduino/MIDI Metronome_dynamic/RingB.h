class RingB : public Ring {
public:
  RingB()
    : Ring() {
    drawBeatBackground();
  }

private:
  int beat;
  int direction;

  virtual void startHandler() {
    clear();
    direction = 1;
    beat = 1;
  }

  virtual void stopHandler() {
    drawBeatBackground();
  }

  virtual void tickHandler(int count) {
    drawBeatBackground();
    drawBeatIndicator();
    drawPendulum(count);
  }

  virtual void beatStartedHandler() {
  }   

  virtual void beatEndedHandler() {
    // Beat is over, change direction
    direction *= -1;

    // Advance beat
    beat++;

    // Start over (oops, this can only handle 4/n time signatures!)
    if (beat == 5) {
      beat = 1;
    }
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
    pixels[index]->set(160, 20, 20);
  }

  void drawBeatIndicator() {
    // Beat indicator
    int beatIndicatorBrightness = 100;
    int beatPixelIndex;
    switch (beat) {
      case 1:
        beatPixelIndex = 1;
        break;
      case 2:
        beatPixelIndex = 0;
        break;
      case 3:
        beatPixelIndex = 23;
        break;
      case 4:
        beatPixelIndex = 22;
        break;
    }

    pixels[beatPixelIndex]->set(0, 100, 0);
  }

  void drawBeatBackground() {
    // Beat background
    pixels[1]->set(0, 0, 10);
    pixels[0]->set(0, 0, 10);
    pixels[23]->set(0, 0, 10);
    pixels[22]->set(0, 0, 10);
  }
};