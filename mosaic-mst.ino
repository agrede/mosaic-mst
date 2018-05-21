#include <SPI.h>
#include "PID_v1.h"
#include <CmdMessenger.h>
#include "encoder.h"
#include "movement.h"
#include "wiredriver.hpp"
#include "spotposition.h"
#include "errorcells.hpp"
#include <RTClib.h>;

CmdMessenger usrMsg = CmdMessenger(Serial);
enum {
    kRecievePIDLogging,
    kGetPIDEnabled,
    kSetPIDEnabled,
    kGetPIDLoggingEnabled,
    kSetPIDLoggingEnabled,
    kGetErrorCellEnabled,
    kSetErrorCellEnabled,
    kSheetCenter,
    kSetTarget,
    kGetTarget,
    kSetErrorScale,
    kGetErrorScale,
    kSetEMOCountThreshold,
    kGetEMOCountThreshold,
    kSetStabilityCountThreshold,
    kGetStabilityCountThreshold,
    kSetStabilityThreshold,
    kGetStabilityThreshold,
    kSetFeedForwardEnabled,
    kGetFeedForwardEnabled,
    kSetGlobalPosition,
    kGetGlobalPosition,
    kSetPanelOrientation,
    kGetPanelOrientation,
    kSetRTC,
    kGetRTC,
};
enum {
    kRequestError,
    kRecieveError,
};
CmdMessenger errMsg = CmdMessenger(Serial1);

double ki[3] = {0.48, 0.375, 0.375};
double kp[3] = {1.0, 0.75, 0.75};
double kd[3] = {0.04, 0.04, 0.04};

ErrorCells errCells = ErrorCells(
    &errMsg, &pos_vector, kRequestError);
Encoders encoders = Encoders();
WireDriver wireDriver = WireDriver(
    &encoders, &usrMsg, &L_target, kRecievePIDLogging, ki, kp, kd);
RTC_DS1307 rtc;
DateTime now;

/* Communication Functions */

void getPIDEnabled() {
    usrMsg.sendBinCmd<bool>(kGetPIDEnabled, wireDriver.enabled()); }
void setPIDEnabled() {
    if (usrMsg.readBinArg<bool>()) {
        wireDriver.startDriver();
    } else {
        wireDriver.stopDriver();
    }
}
void getPIDLoggingEnabled() {
    usrMsg.sendBinCmd<bool>(kGetPIDLoggingEnabled, wireDriver.logging);
}

void setPIDLoggingEnabled() { wireDriver.logging = usrMsg.readBinArg<bool>(); }
void getErrorCellEnabled() {
    usrMsg.sendBinCmd<bool>(kGetErrorCellEnabled, errCells.enabled);
}
void setErrorCellEnabled() { errCells.enabled = usrMsg.readBinArg<bool>(); }
void sheetCenter() { encoders.center(); }
void userSetTarget() {
    for (int i=0;i<3;i++) {
        pos_vector[i] = usrMsg.readBinArg<double>();
    }
    setTarget();
}
void userGetTarget() {
    usrMsg.sendCmdStart(kGetTarget);
    for (int i=0;i<3;i++) {
        usrMsg.sendCmdBinArg<double>(pos_vector[i]);
    }
    usrMsg.sendCmdEnd();
}
void setErrorScale() { errCells.scale = usrMsg.readBinArg<float>(); }
void getErrorScale() {
    usrMsg.sendBinCmd<float>(kGetErrorScale, errCells.scale);
}
void setEMOCountThreshold() {
    wireDriver.emo_count_threshold = usrMsg.readBinArg<byte>();
}
void getEMOCountThreshold() {
    usrMsg.sendBinCmd<uint16_t>(
        kGetEMOCountThreshold,
        (uint16_t)wireDriver.emo_count_threshold);
}
void setStabilityCountThreshold() {
    wireDriver.stability_count_threshold = usrMsg.readBinArg<byte>();
}
void getStabilityCountThreshold() {
    usrMsg.sendBinCmd<uint16_t>(
        kGetStabilityCountThreshold,
        (uint16_t)wireDriver.stability_count_threshold);
}
void setStabilityThreshold() {
    wireDriver.stability_threshold = usrMsg.readBinArg<double>();
}
void getStabilityThreshold() {
    usrMsg.sendBinCmd<double>(kGetStabilityThreshold,
                              wireDriver.stability_threshold);
}
void setFeedForwardEnabled() { feed_forward = usrMsg.readBinArg<bool>(); }
void getFeedForwardEnabled() {
    usrMsg.sendBinCmd<bool>(kGetFeedForwardEnabled, feed_forward);
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
    double heading = usrMsg.readBinArg<double>();
    double tilt = usrMsg.readBinArg<double>();
    updatePanel(heading, tilt);
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

void unknownCommand(){
    Serial.println("Unknown!");
}

void unknownCommandErr(){
    Serial1.println("Unknown!");
}

void recieveError() { errCells.updateOffsets(); }

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    if (! rtc.begin()) {
        Serial.println("No RTC");
        while (1);
    } else {
        Serial.println("RTC Enabled");
    }
    // encoders.begin();
    Serial.println("Encoders Started");
    wireDriver.begin();
    Serial.println("Wiredriver Started");
    now = rtc.now();
    initializeSpot(0.0, 0.0, 40.7946, -77.8647,
                   now.year(), now.month(), now.day(),
                   double(now.hour())+double(now.minute())/60.0
                   +double(now.second())/3600.0);
    Serial.println("Spot init");
    setL0();
    Serial.println("Set L0");
    usrMsg.attach(unknownCommand);
    usrMsg.attach(kGetPIDEnabled, getPIDEnabled);
    usrMsg.attach(kSetPIDEnabled, setPIDEnabled);
    usrMsg.attach(kGetPIDLoggingEnabled, getPIDLoggingEnabled);
    usrMsg.attach(kSetPIDLoggingEnabled, setPIDLoggingEnabled);
    usrMsg.attach(kGetErrorCellEnabled, getErrorCellEnabled);
    usrMsg.attach(kSetErrorCellEnabled, setErrorCellEnabled);
    usrMsg.attach(kSheetCenter, sheetCenter);
    usrMsg.attach(kSetTarget, userSetTarget);
    usrMsg.attach(kGetTarget, userGetTarget);
    usrMsg.attach(kSetErrorScale, setErrorScale);
    usrMsg.attach(kGetErrorScale, getErrorScale);
    usrMsg.attach(kSetEMOCountThreshold, setEMOCountThreshold);
    usrMsg.attach(kGetEMOCountThreshold, getEMOCountThreshold);
    usrMsg.attach(kSetStabilityCountThreshold, setStabilityCountThreshold);
    usrMsg.attach(kGetStabilityCountThreshold, getStabilityCountThreshold);
    usrMsg.attach(kSetStabilityThreshold, setStabilityThreshold);
    usrMsg.attach(kGetStabilityThreshold, getStabilityThreshold);
    usrMsg.attach(kSetFeedForwardEnabled, setFeedForwardEnabled);
    usrMsg.attach(kGetFeedForwardEnabled, getFeedForwardEnabled);
    usrMsg.attach(kSetGlobalPosition, setGlobalPosition);
    usrMsg.attach(kGetGlobalPosition, getGlobalPosition);
    usrMsg.attach(kSetPanelOrientation, setPanelOrientation);
    usrMsg.attach(kGetPanelOrientation, getPanelOrientation);
    usrMsg.attach(kGetRTC, getRTC);
    usrMsg.attach(kSetRTC, setRTC);
    errMsg.attach(unknownCommandErr);
    errMsg.attach(kRecieveError, recieveError);
    Serial.println("Attached callbacks");
}

void loop() {
    usrMsg.feedinSerialData();
    errMsg.feedinSerialData();
    if (wireDriver.enabled()) {
        if (encoders.enabled) {
            encoders.read();
        }
        if (wireDriver.stable()) {
            errCells.loop();
        }
        wireDriver.loop();
    }
}
