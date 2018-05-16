#include <SPI.h>
#include "PID_v1.h"
#include <SoftwareSerial.h>
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
    kStartPID,
    kStopPID,
    kStartError,
    kStopError,
    kSheetCenter,
    kSetTarget,
    kUpdateOffsets,
};
SoftwareSerial errSerial(9, 2); //rx, tx
CmdMessenger errMsg = CmdMessenger(errSerial);

double ki[3] = {0.48, 0.375, 0.375};
double kp[3] = {1.0, 0.75, 0.75};
double kd[3] = {0.04, 0.04, 0.04};

ErrorCells errCells = ErrorCells(&errMsg, &pos_vector);
Encoders encoders;
WireDriver wireDriver = WireDriver(&encoders, &usrMsg, &L_target, kRecievePIDLogging, ki, kp, kd);
RTC_DS1307 rtc;
DateTime now;


void userSetTarget() {
    double newPos[3];
    for (int i=0;i<3;i++) {
        newPos[i] = usrMsg.readDoubleArg();
    }
    setTarget(newPos);
}
void startDriver() {wireDriver.startDriver();}
void stopDriver() {wireDriver.stopDriver();}
void startError() {errCells.start();}
void stopError() {errCells.stop();}
void encCenter() {encoders.center();}

void setup() {
    Serial.begin(115200);
    errSerial.begin(115200);
    rtc.begin();
    now = rtc.now();
    initializeSpot(0.0, 0.0, 40.7946, -77.8647,
                   now.year(), now.month(), now.day(),
                   double(now.hour())+double(now.minute())/60.0
                   +double(now.second())/3600.0);
    setL0();
    usrMsg.attach(kStartPID, startDriver);
    usrMsg.attach(kStopPID, stopDriver);
    usrMsg.attach(kStartError, startError);
    usrMsg.attach(kStopError, stopError);
    usrMsg.attach(kSheetCenter, encCenter);
    usrMsg.attach(kSetTarget, userSetTarget);
}

void loop() {
    usrMsg.feedinSerialData();
    errMsg.feedinSerialData();
    if (encoders.enabled) {
        encoders.read();
    }
    if (wireDriver.stable()) {
        errCells.loop();
    }
    wireDriver.loop();
}
