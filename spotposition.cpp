#include "spotposition.hpp"

sunpos sun_pos;
RTC_DS1307 rtc;
DateTime now;

/* Panel movement */
double panel_angles[2]; /* heading WoS, tilt in rad*/

/* Predictive */
double spot_position[2];         /* x and y in mm */
double spot_last_position[2];    /* x and y in mm */

/* Aux variables */
double sin_heading, sin_tilt, sin_azimuth, sin_zenith;
double cos_heading, cos_tilt, cos_azimuth, cos_zenith;
bool feed_forward = false;

void spotSetup() {
    rtc.begin();
    sun_pos.Dt = 68.9677;       // As of 2018
    sun_pos.Longitude = rad*-77.8647;
    sun_pos.Latitude = rad*40.7946;
    spotUpdateOrientation();
    usrMsg.attach(kSetFeedForward, setFeedForward);
    usrMsg.attach(kGetFeedForward, getFeedForward);
    usrMsg.attach(kSetGlobalPosition, setGlobalPosition);
    usrMsg.attach(kGetGlobalPosition, getGlobalPosition);
    usrMsg.attach(kSetPanelOrientation, setPanelOrientation);
    usrMsg.attach(kGetPanelOrientation, getPanelOrientation);
    usrMsg.attach(kGetRTC, getRTC);
    usrMsg.attach(kSetRTC, setRTC);
}

void spotUpdateOrientation() {
    sin_heading = sin(panel_angles[0]);
    cos_heading = cos(panel_angles[0]);
    sin_tilt = sin(panel_angles[1]);
    cos_tilt = cos(panel_angles[1]);
}

void spotLoop() {
    if (!feed_forward) { return; }

    // Update Solar Angle -----------------------
    // Set current time
    now = rtc.now();
    sun_pos.Year = now.year();
    sun_pos.Month = now.month();
    sun_pos.Day = now.day();
    sun_pos.UT = double(now.hour())+double(now.minute())/60.0
        +double(now.second())/3600.0;

    // Calculate Azimuth angle and Zenith angle
    sun_pos.Algorithm2('l');    // Fastest with sufficient accuracy

    // Update sin and cosines
    sin_azimuth = sin(double(sun_pos.Azimuth));
    sin_zenith = sin(double(sun_pos.Zenith));
    cos_azimuth = cos(double(sun_pos.Azimuth));
    cos_zenith = cos(double(sun_pos.Zenith));
    spot_last_position[0] = spot_position[0];
    spot_last_position[1] = spot_position[1];

    // Calculate spot position----------------------
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

void setFeedForward() { feed_forward = usrMsg.readBinArg<bool>(); }
void getFeedForward() {
    usrMsg.sendBinCmd<bool>(kGetFeedForward, feed_forward);
}
void setGlobalPosition() {
    sun_pos.Longitude = rad*usrMsg.readBinArg<double>();
    sun_pos.Latitude = rad*usrMsg.readBinArg<double>();
}
void getGlobalPosition() {
    usrMsg.sendCmdStart(kGetGlobalPosition);
    usrMsg.sendCmdBinArg<double>(sun_pos.Longitude/rad);
    usrMsg.sendCmdBinArg<double>(sun_pos.Latitude/rad);
    usrMsg.sendCmdEnd();
}
void setPanelOrientation() {
    panel_angles[0] = rad*usrMsg.readBinArg<double>();
    panel_angles[1] =  rad*usrMsg.readBinArg<double>();
    spotUpdateOrientation();
}
void getPanelOrientation() {
    usrMsg.sendCmdStart(kGetGlobalPosition);
    usrMsg.sendCmdBinArg<double>(panel_angles[0]/rad);
    usrMsg.sendCmdBinArg<double>(panel_angles[1]/rad);
    usrMsg.sendCmdEnd();
}
void setRTC() {
    rtc.adjust(DateTime(usrMsg.readBinArg<uint32_t>()));
}
void getRTC() {
    now = rtc.now();
    usrMsg.sendBinCmd<uint32_t>(kGetRTC, now.unixtime());
}
