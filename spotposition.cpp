#include "spotposition.h"

sunpos sun_pos;
/* Panel movement */
double panel_angles[2]; /* heading WoS, tilt in rad*/

/* Predictive */
double spot_position[2];         /* x and y in mm */
double spot_last_position[2];    /* x and y in mm */

/* Aux variables */
double sin_heading, sin_tilt, sin_azimuth, sin_zenith;
double cos_heading, cos_tilt, cos_azimuth, cos_zenith;

void initializeSpot(double heading, double tilt, double latitude,
                    double longitude, int year, int month, int day, double hour) {
    sun_pos.Year = year;
    sun_pos.Month = month;
    sun_pos.Day = day;
    sun_pos.UT = hour;
    sun_pos.Dt = 68.9677;       // As of 2018
    sun_pos.Longitude = rad*longitude;
    sun_pos.Latitude = rad*latitude;
    updatePannel(heading, latitude);
}

void updatePannel(double heading, double tilt) {
    panel_angles[0] = rad*heading;
    panel_angles[1], rad*tilt;
    sin_heading = sin(panel_angles[0]);
    cos_heading = cos(panel_angles[0]);
    sin_tilt = sin(panel_angles[1]);
    cos_tilt = cos(panel_angles[1]);
}

void calculateSpot() {
    // Unit vector components pointing to sun in panel coordinates
    double x = (cos_azimuth*sin_zenith*sin_heading
               -sin_azimuth*sin_zenith*cos_heading);
    double y = (cos_zenith*sin_tilt
               -sin_azimuth*sin_zenith*cos_tilt*sin_heading
               -cos_azimuth*sin_zenith*cos_tilt*sin_heading);
    // Sin of angle between sun and panel normal
    double r = sqrt(
        1.0-sq(cos_zenith*sin_tilt
             +sin_azimuth*sin_zenith*cos_tilt*sin_heading
             +cos_azimuth*sin_zenith*cos_tilt*sin_heading));

    // x = (c0*r^8+c1*r)*cos(rotation about normal)
    spot_position[0] = (spot_coefficients[0]*pow(r, 7)
                        +spot_coefficients[1])*x;
    // y = (c0*r^8+c1*r)*sin(rotation about normal)
    spot_position[1] = (spot_coefficients[0]*pow(r, 7)
                        +spot_coefficients[1])*y;
}

void updateSpotPredictions(int year, int month, int day, double hour) {
    // Set current time
    sun_pos.Year = year;
    sun_pos.Month = month;
    sun_pos.Day = day;
    sun_pos.UT = hour;

    // Calculate Azimuth angle and Zenith angle
    sun_pos.Algorithm2('l');    // Fastest with sufficient accuracy

    // Update sin and cosines
    sin_azimuth = sin(double(sun_pos.Azimuth));
    sin_zenith = sin(double(sun_pos.Zenith));
    cos_azimuth = cos(double(sun_pos.Azimuth));
    cos_zenith = cos(double(sun_pos.Zenith));
    spot_last_position[0] = spot_position[0];
    spot_last_position[1] = spot_position[1];

    // Calculate spot position
    calculateSpot();
}
