#include <spotposition.h>
#include <math.h>
#include "Sun_position_algorithms.h"


float pannel_angles[2] = {0.0, 0.7155849933176751};
float spot_coefficients[2] = {1.4931946984090267, 5.590295459232854};
sunpos sun_pos;

void initializeSpot(float heading, float tilt, float latitude,
                    float longitude, int year, int month, int day, double hour) {
    sun_pos.Year = year;
    sun_pos.Month = month;
    sun_pos.Day = day;
    sun_pos.UT = hour;
    sun_pos.Dt = 68.9677;       // As of 2018
    sun_pos.Longitude = rad*longitude;
    sun_pos.Latitude = rad*latitude;
    updatePannel(heading, latitude);
}

void updatePannel(float heading, float tilt) {
    pannel_angles = {rad*heading, rad*tilt};
    sin_heading = sin(pannel_angles[0]);
    cos_heading = cos(pannel_angles[0]);
    sin_tilt = sin(pannel_angles[1]);
    cos_tilt = cos(pannel_angles[1]);
}

void calculateSpot() {
    // Unit vector components pointing to sun in panel coordinates
    float x = (cos_azimuth*sin_zenith*sin_heading
               -sin_azimuth*sin_zenith*cos_heading);
    float y = (cos_zenith*sin_tilt
               -sin_azimuth*sin_zenith*cos_tilt*sin_heading
               -cos_azimuth*sin_zenith*cos_tilt*sin_heading);
    // Sin of angle between sun and panel normal
    float r = sqrt(
        1-sq(cos_zenith*sin_tilt
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
    sin_azimuth = sin(float(sun_pos.Azimuth));
    sin_zenith = sin(float(sun_pos.Zenith));
    cos_azimuth = cos(float(sun_pos.Azimuth));
    cos_zenith = cos(float(sun_pos.Zenith));
    spot_last_position[0] = spot_position[0];
    spot_last_position[1] = spot_position[1];

    // Calculate spot position
    calculateSpot();
}
