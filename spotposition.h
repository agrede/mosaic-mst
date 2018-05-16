#ifndef SPOTPOSITION_H
#define SPOTPOSITION_H

#include "Arduino.h"
#include <math.h>
#include "Sun_position_algorithms.h"

extern sunpos sun_pos;

const double rad = 0.017453292519943295;

/* Panel movement */
extern double panel_angles[2]; /* heading WoS, tilt in rad*/

/* Predictive */
const double spot_coefficients[2] = {1.4931946984090267, 5.590295459232854};      /* a[0]*sin^8, a[1]*sin in mm */
extern double spot_position[2];         /* x and y in mm */
extern double spot_last_position[2];    /* x and y in mm */

/* Aux variables */
extern double sin_heading, sin_tilt, sin_azimuth, sin_zenith;
extern double cos_heading, cos_tilt, cos_azimuth, cos_zenith;

void calculateSpot();
void initializeSpot(double heading, double tilt, double latitude,
                    double longitude, int year, int month, int day, double hour);
void updateSpotPrediction(int year, int month, int day, double hour);
void updatePannel(double heading, double tilt);

#endif
