#ifndef ENCODER_HPP
#define ENCODER_HPP

#include "Arduino.h"
#include "coms.hpp"
#include <SPI.h>


extern double encoder_counts[3];
extern bool encoder_enabled;
extern bool encoder_read;
extern byte encoder_stats[3];

const int encoder_select[3] = {8, 4, 7};
const SPISettings SPISET = SPISettings(5000000, MSBFIRST, SPI_MODE0);

bool encoderStart();
bool encoderStop();
void encoderClear();
void encoderRead();
void encoderStatus();

void encoderSetup();

void getEncoder();
void setEncoder();
void getEncoderCounts();
void getEncoderStatus();

#endif
