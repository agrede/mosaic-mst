#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "coms.hpp"
#include <Adafruit_MotorShield.h>
#include "movement.hpp"

const byte AFMSBOT = 0x61;
const byte AFMSTOP = 0x60;
const char sm_steptype = DOUBLE;
const int sm_dir[3] = {-1, 1, 1};

extern bool sm_enabled;
extern bool sm_stable;
extern bool sm_log;

void smSetup();
void smLoop();

void smRemSteps();
void smTakeSteps();

void sendSMLog();
void getSMEnable();
void setSMEnable();
void getSMLogEnable();
void setSMLogEnable();

#endif
