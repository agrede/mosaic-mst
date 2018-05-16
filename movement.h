#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Arduino.h"
#include <math.h>

const double wire_resolution;   /* (count or step)/mm */
/* Frame pivot points in mm */
const double x_frame[3] = {50.0, 25.0, -90.0};
const double y_frame[3] = {90.0, 90.0, 0.0};
const double x_sheet[3] = {70.0, -25.0, -70.0};
const double y_sheet[3] = {70.0, 70.0, 70.0};
const double pos_limits[3] = {7.0, 7.0, 0.08726646259971647};

extern double pos_vector[3];

extern long L_target[3];
extern long L_0[3];

void setTarget(double pos[3]);
void setL0();


#endif
