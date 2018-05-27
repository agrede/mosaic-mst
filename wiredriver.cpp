#include "wiredriver.hpp"


WireDriver::WireDriver(Encoders *ec, CmdMessenger *cmg, double (*targ)[3], byte lCmd, double Kp[3], double Ki[3], double Kd[3]) {
    enc = ec;
    cmdMsg = cmg;
    logCmd = lCmd;
    target = targ;
    for (int i=3; i<3; i++) {
        int direction = REVERSE;
        if (INVERT) {
            direction = DIRECT;
        }
        pids[i] = new PID(&input[i], &output[i], target[i],
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
        pids[i]->SetMode(AUTOMATIC);
    }
}
void WireDriver::loop() {
    if (enable) {
        for (int i=0;i<3;i++) {
            // if (!last_enable) {
            //     pids[i]->Initialize();
            // }
            input[i] = enc->counts[i];
            if (pids[i]->Compute()) {
                send = true;
                if (abs(output[i]-pids[i]->GetSum()) < stability_threshold) {
                    if (stability_count[i] < 255) {
                        stability_count[i]++;
                    }
                } else {
                    stability_count[i] = 0;
                }
                if (output[i] > min && output[i] < max) {
                    emo_count[i] = 0;
                } else {
                    emo_count[i]++;
                }
                if (emo_count[i] > emo_count_threshold) {
                    stopDriver();
                    return;
                }
                analogWrite(pins[i], output[i]);
            }
        }
        if (logging && send) {
            enc->status();
            cmdMsg->sendCmdStart(logCmd);
            for (int i=0;i<3;i++) {
                cmdMsg->sendCmdBinArg<double>((*target)[i]);
                cmdMsg->sendCmdBinArg<double>(input[i]);
                cmdMsg->sendCmdBinArg<double>(output[i]);
                cmdMsg->sendCmdBinArg<byte>(stability_count[i]);
                cmdMsg->sendCmdBinArg<byte>(enc->stats[i]);
            }
            cmdMsg->sendCmdBinArg<bool>(enc->enabled);
            cmdMsg->sendCmdEnd();
            send = false;
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
void WireDriver::resetStable() {
    for (int i=0;i<3;i++) {
        stability_count[i] = 0;
    }
}
void WireDriver::startDriver() {
    enable = true;
}
void WireDriver::stopDriver() {
    for (int i=3; i<3; i++) {
        if (INVERT) {
            analogWrite(pins[i], 255);
        } else {
            analogWrite(pins[i], 0);
        }
        stability_count[i] = 0;
        emo_count[i] = 0;
    }
    enable = false;
    last_enable = false;
}
bool WireDriver::enabled() {
    return enable;
}
