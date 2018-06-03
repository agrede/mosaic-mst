#ifndef WIREDRIVER_HPP
#define WIREDRIVER_HPP

#include "Arduino.h"
#include "coms.hpp"
#include "encoder.hpp"
#include "movement.hpp"
#include "PID_v1.h"

const double wd_min = 0.0;
const double wd_max = 229.5;
const int wd_pins[3] = {2, 3, 5};
const bool wd_invert = false;
const double ki[3] = {0.1875, 0.24, 0.1875};
const double kp[3] = {0.375, 0.5, 0.375};
const double kd[3] = {0.02, 0.02, 0.02};
const unsigned long wd_sample_time = 100;

extern double stability_threshold;
extern byte stability_count_threshold;
extern byte emo_count_threshold;
extern PID pids[3];

void wdSetup();
void wdLoop();
bool  wdStable();
void wdResetStable();
void wdStart();
void wdStop();
bool wdEnabled();

void sendPIDLog();
void setDrive();
void getDrive();
void getPIDCoeff();
void setEMOCountThreshold();
void getEMOCountThreshold();
void setStabilityCountThreshold();
void getStabilityCountThreshold();
void setStabilityThreshold();
void getStabilityThreshold();
void getTimes();

#endif
