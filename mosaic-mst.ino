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

ErrorCells errCells;
Encoders encoders;
WireDriver wireDriver;
RTC_DS1307 rtc;
DateTime now;


void userSetTarget() {
    float newPos[3];
    for (int i=0;i<3;i++) {
        newPos[i] = usrMsg.readFloatArg();
    }
    setTarget(newPos);
}

void setup() {
    Serial.begin(115200);
    errSerial.begin(115200);
    errCells = ErrorCells(&errMsg, &offsets);
    encoders = Encoders();
    rtc.begin();
    now = rtc.now();
    initializeSpot(0.0, 0.0, 40.7946, -77.8647,
                   now.year(), now.month(), now.day(),
                   double(now.hour())+double(now.minute())/60.0
                   +double(now.second())/3600.0);
    setL0();
    setTarget({0.0, 0.0, 0.0});
    wireDriver = WireDriver(
        &encoders, &usrMsg, kRecievePIDLogging, &L_target,
        {0.48, 0.375, 0.375}, {1.0, 0.75, 0.75}, {0.04, 0.04, 0.04});
    usrMsg.attach(kStartPID, wireDriver.start);
    usrMsg.attach(kStopPID, wireDriver.stop);
    usrMsg.attach(kStartError, errorCells.start);
    usrMsg.attach(kStopError, errorCells.stop);
    usrMsg.attach(kSheetCenter, encoders.center);
    usrMsg.attach(kSetTarget, userSetTarget);
}

void loop() {
    usrMsg.feedinSerialData();
    errSerial.feedinSerialData();
    if (encoders.enabled) {
        encoders.read();
    }
    if (wireDriver.stable()) {
        errorCells.loop();
    }
    wireDriver.loop();
}
