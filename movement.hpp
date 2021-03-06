#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include "Arduino.h"
#include "coms.hpp"

const double wire_resolution = 2371.75;   /* (count or step)/mm */
/* Frame pivot points in mm */
const double x_frame[3] = { 50.930,  24.070, -90.000};
const double y_frame[3] = { 90.000,  90.000,   0.930};
const double x_sheet[3] = { 69.685, -24.020, -69.685};
const double y_sheet[3] = { 70.000,  70.000,  70.000};
const double pos_limits[3] = {7.0, 7.0, 0.08726646259971647};

extern double pos_vector[3];
extern bool pos_updated;

extern double L_target[3];
extern double L_0[3];

void setTarget();
void setL0();

void userSetTarget();
void userGetTarget();

void movementSetup();

#endif
