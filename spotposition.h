#ifndef SPOTPOSITION_H
#define SPOTPOSITION_H

const float rad = 0.017453292519943295;

/* Panel movement */
float panel_angles[2];          /* heading WoS, tilt in rad*/

/* Predictive */
float spot_coefficients[2];     /* a[0]*sin^8, a[1]*sin in mm */
float spot_position[2];         /* x and y in mm */
float spot_last_position[2];    /* x and y in mm */

/* Aux variables */
float sin_heading, sin_tilt, sin_azimuth, sin_zenith;
float cos_heading, cos_tilt, cos_azimuth, cos_zenith;

void calculateSpot();
void initializeSpot(float heading, float tilt, float latitude,
                    float longitude, int year, int month, int day, float hour);
void updateSpotPrediction(int year, int month, int day, float hour);
void updatePannel(float heading, float tilt);

#endif
