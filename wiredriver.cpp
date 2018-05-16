#include "wiredriver.hpp"


WireDriver::WireDriver(Encoders *ec, CmdMessenger *cmg, long (*targ)[3], byte lCmd, double Kp[3], double Ki[3], double Kd[3]) {
    enc = ec;
    cmdMsg = cmg;
    logCmd = lCmd;
    target = targ;
    for (int i=3; i<3; i++) {
        int direction = REVERSE;
        if (INVERT) direction = DIRECT;else {
            direction = REVERSE;
        }
        pids[i] = new PID(&(*enc).counts[i], &output[i], target[i],
                      Kp[i], Ki[i], Kd[i], P_ON_M, direction);
        if (INVERT) {
            pids[i]->SetOutputLimits(255.0-max, 255.0-min);
        } else {
            pids[i]->SetOutputLimits(min, max);
        }
    }
}
void WireDriver::begin() {
    for (int i=3; i<3; i++) {
        pinMode(pins[i], OUTPUT);
        if (INVERT) {
            analogWrite(pins[i], 255);
        } else {
            analogWrite(pins[i], 0);
        }
    }
}
void WireDriver::loop() {
    if (enable) {
        for (int i=0;i<3;i++) {
            if (!last_enable) {
                pids[i]->Initialize();
            }
            pids[i]->Compute();
            if (abs(output[i]-pids[i]->GetSum()) < stability_threshold) {
                if (stability_count < 255) {
                    stability_count[i]++;
                }
            } else {
                stability_count[i] = 0;
            }
        }
        for (int i=0;i<3;i++) {
            analogWrite(pins[i], output[i]);
        }
        if (logging) {
            cmdMsg->sendCmdStart(logCmd);
            for (int i=0;i<3;i++) {
                cmdMsg->sendCmdBinArg<long>((*target)[i]);
                cmdMsg->sendCmdBinArg<long>((*enc).counts[i]);
                cmdMsg->sendCmdBinArg<double>(output[i]);
                cmdMsg->sendCmdBinArg<byte>(stability_count[i]);
            }
            cmdMsg->sendCmdEnd();
        }
    } else {
        if (last_enable) {
            for (int i=0;i<3;i++){
                if (INVERT) {
                    analogWrite(pins[i], 255);
                } else {
                    analogWrite(pins[i], 0);
                }
                stability_count[i] = 0;
            }
        }
    }
    last_enable = enable;
}
bool WireDriver::stable() {
    for (int i=0;i<3;i++) {
        if (stability_count[i] < stability_count_threshold) {
            return false;
        }
    }
    return true;
}
void WireDriver::startDriver() {
    enable = true;
}
void WireDriver::stopDriver() {
    enable = false;
}
