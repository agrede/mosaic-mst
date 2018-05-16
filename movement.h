#ifndef MOVEMENT_H
#define MOVEMENT_H

const float wire_resolution;   /* (count or step)/mm */
/* Frame pivot points in mm */
const float x_frame[3] = {50.0, 25.0, -90.0};
const float y_frame[3] = {90.0, 90.0, 0.0};
const float x_sheet[3] = {70.0, -25.0, -70.0};
const float y_sheet[3] = {70.0, 70.0, 70.0};
const float pos_limits[3] = {7.0, 7.0, 0.08726646259971647};

float pos_vector[3];

long L_target[3];
long L_0[3];

void setTarget(float pos[3]);


#endif
