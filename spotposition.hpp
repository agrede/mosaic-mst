#ifndef SPOTPOSITION_HPP
#define SPOTPOSITION_HPP

#include "Arduino.h"
#include "coms.hpp"
#include "Sun_position_algorithms.h"
#include <RTClib.h>;

extern sunpos sun_pos;
extern RTC_DS1307 rtc;
extern DateTime now;

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
extern bool feed_forward;

void spotSetup();
void spotLoop();
void spotUpdateOrientation();

void setFeedForward();
void getFeedForward();
void getGlobalPosition();
void setGlobalPosition();
void getPanelOrientation();
void setPanelOrientation();
void getRTC();
void setRTC();

#endif
