#ifndef ENCODER_H
#define ENCODER_H

#include "Arduino.h"
#include <SPI.h>


class Encoders {
public:
    long counts[3];
    bool enabled = true;
    Encoders();
    void begin();
    void start();
    void stop();
    void clear();
    void read();
    void center();
private:
    const int select[3] = {8, 4, 7};
    const SPISettings SPISET = SPISettings(5000000, MSBFIRST, SPI_MODE0);
};

#endif
