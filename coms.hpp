#ifndef COMS_HPP
#define COMS_HPP

#include "Arduino.h"
#include <CmdMessenger.h>

enum {
    kRecievePIDLogging,
    kSetEncoder,
    kGetEncoder,
    kSetDrive,
    kGetDrive,
    kSetLogging,
    kGetLogging,
    kSetErrorCell,
    kGetErrorCell,
    kSetEncoderCenter,
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
    kSetFeedForward,
    kGetFeedForward,
    kSetGlobalPosition,
    kGetGlobalPosition,
    kSetPanelOrientation,
    kGetPanelOrientation,
    kSetRTC,
    kGetRTC,
    kGetEncoderCounts,
    kGetEncoderStats,
    kGetPIDCoeff,
    kGetTimes,
    kSetDemo,
    kGetDemo,
};
enum {
    kRequestError,
    kRecieveError,
};

extern CmdMessenger usrMsg;
extern CmdMessenger errMsg;
extern bool logging;

void comsSetup();
void comsLoop();

#endif
