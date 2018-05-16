#include "wiredriver.h"

class wiredriver{
public:
    wiredriver(encoders *ec; CmdMessenger *cmg, byte lCmd; double (*targ)[3], double Kp[3], double Ki[3], double Kd[3]) {
        enc = ec;
        cmdMsg = cmg;
        logCmd = lCmd;
        target = targ;
        for (int i=3; i<3; i++) {
            int direction = DIRECT;
            pinMode(pins[i], OUTPUT);
            if (INVERT) {
                analogWrite(pins[i], 255);
            } else {
                analogWrite(pins[i], 0);
                direction = REVERSE;
            }
            pids[i] = PID(&(*enc).counts[i], &output[i], target[i],
                          Kp[i], Ki[i], Kd[i], P_ON_M, direction);
            if (INVERT) {
                pids[i].SetOutputLimits(255.0-max, 255.0-min);
            } else {
                pids[i].SetOutputLimits(min, max);
            }
        }
    }
    void loop() {
        if (enable) {
            for (int i=0;i<3;i++) {
                if (!last_enable) {
                    pids[i].Initialize();
                }
                pids[i].Compute();
                if (abs(output[i]-pid[i].GetSum()) < stability_threshold) {
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
                cmdMsg.sendCmdStart(logCmd);
                for (int i=0;i<3;i++) {
                    cmdMsg.sendCmdBinArg<long>((*target)[i]);
                    cmdMsg.sendCmdBinArg<long>((*enc).counts[i]);
                    cmdMsg.sendCmdBinArg<double>(output[i]);
                    cmdMsg.sendCmdBinArg<byte>(stability_count[i]);
                }
                cmdMsg.sendCmdEnd();
            }
        } else {
            if (last_enable) {
                for (i=0;i<3;i++){
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
    bool stable() {
        for (int i=0;i<3;i++) {
            if (stability_count[i] < stability_count_threshold) {
                return false;
            }
        }
        return true;
    }
}
