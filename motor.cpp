#include "motor.hpp"

Adafruit_MotorShield AFMSbot(AFMSBOT);
Adafruit_MotorShield AFMStop(AFMSTOP);

long sm_position[3] = {0, 0, 0};
int sm_remsteps[3] = {0, 0, 0};
int sm_minstep = 0;

Adafruit_StepperMotor *sm_motors[3] = {
    AFMStop.getStepper(200, 1),
    AFMStop.getStepper(200, 2),
    AFMSbot.getStepper(200, 1)
};

bool sm_enabled = false;
bool sm_log = false;
bool sm_stable = true;

void smSetup() {
    AFMSbot.begin();
    AFMStop.begin();
    for (int i=0; i<3; i++) {
        sm_motors[i]->setSpeed(2);
    }
    usrMsg.attach(kGetSMEnable, getSMEnable);
    usrMsg.attach(kSetSMEnable, setSMEnable);
    usrMsg.attach(kGetSMLogEnable, getSMLogEnable);
    usrMsg.attach(kSetSMLogEnable, setSMLogEnable);
}

void smLoop() {
    if (sm_enabled) {
        smRemSteps();
        if (!sm_stable) {
            smTakeSteps();
        }
    }
}

void smRemSteps() {
    sm_minstep = 0;
    for (int i=0; i<3; i++) {
        sm_remsteps[i] = int(long(L_target[i]) - sm_position[i]);
        if (sm_minstep == 0 || abs(sm_remsteps[i]) < sm_minstep) {
            sm_minstep = abs(sm_remsteps[i]);
        }
    }
    sm_stable = sm_minstep > 0;
}

void smTakeSteps() {
    for (int i=0; i<3; i++) {
        int steps = sm_dir[i]*sm_remsteps[i]/sm_minstep;
        int dir = steps < 0 ? BACKWARD : FORWARD;
        sm_motors[i]->step(abs(steps), dir, sm_steptype);
        sm_position[i] += steps;
    }
}

void getSMEnable() { usrMsg.sendBinCmd<bool>(kGetSMEnable, sm_enabled); }
void setSMEnable() {
    if (usrMsg.readBinArg<bool>()) {
        sm_enabled = true;
    } else {
        sm_enabled = false;
    }
}
void getSMLogEnable() { usrMsg.sendBinCmd<bool>(kGetSMLogEnable, sm_enabled); }
void setSMLogEnable() {
    if (usrMsg.readBinArg<bool>()) {
        sm_log = true;
    } else {
        sm_log = false;
    }
}

void sendSMLog() {
    usrMsg.sendCmdStart(kRecieveSMLogging);
    for (int i=0; i<3; i++) {
        usrMsg.sendCmdBinArg<double>(L_target[i]);
        usrMsg.sendCmdBinArg<long>(sm_position[i]);
        usrMsg.sendCmdBinArg<int>(sm_remsteps[i]);
    }
    usrMsg.sendCmdBinArg<int>(sm_minstep);
    usrMsg.sendCmdBinArg<bool>(sm_stable);
    usrMsg.sendCmdEnd();
}
