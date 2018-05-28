#include "wiredriver.hpp"

double stability_threshold = 2.0;
byte stability_count_threshold = 20;
byte emo_count_threshold = 250;
double output[3] = {0.0, 0.0, 0.0};
byte stability_count[3] = {0, 0, 0};
byte emo_count[3] = {0, 0, 0};
bool wd_send = false;
bool wd_enabled = false;
unsigned long wd_last_time;


PID pids[3] = {
    PID(&encoder_counts[0], &output[0], &L_target[0], kp[0], ki[0], kd[0], P_ON_M, REVERSE),
    PID(&encoder_counts[1], &output[1], &L_target[1], kp[1], ki[1], kd[1], P_ON_M, REVERSE),
    PID(&encoder_counts[2], &output[2], &L_target[2], kp[2], ki[2], kd[2], P_ON_M, REVERSE)
};

void wdSetup(){
    unsigned long ct = millis();
    if (ct < wd_sample_time) {
        delay(wd_sample_time-ct);
    }
    ct = millis;
    wd_last_time = ct-wd_sample_time;
    for (int i=0;i<3;i++) {
        pinMode(wd_pins[i], OUTPUT);
        if (wd_invert) {
            pids[i].SetOutputLimits(255.0-wd_max, 255.0-wd_min);
            analogWrite(wd_pins[i], 255);
        } else {
            pids[i].SetOutputLimits(wd_min, wd_max);
            analogWrite(wd_pins[i], 0);
        }
        pids[i].SetMode(AUTOMATIC);
        pids[i].SetSampleTime(wd_sample_time);
    }
    usrMsg.attach(kSetDrive, setDrive);
    usrMsg.attach(kGetDrive, getDrive);
    usrMsg.attach(kGetPIDCoeff, getPIDCoeff);
    usrMsg.attach(kSetEMOCountThreshold, setEMOCountThreshold);
    usrMsg.attach(kGetEMOCountThreshold, getEMOCountThreshold);
    usrMsg.attach(kSetStabilityCountThreshold, setStabilityCountThreshold);
    usrMsg.attach(kGetStabilityCountThreshold, getStabilityCountThreshold);
    usrMsg.attach(kSetStabilityThreshold, setStabilityThreshold);
    usrMsg.attach(kGetStabilityThreshold, getStabilityThreshold);
    usrMsg.attach(kGetTimes, getTimes);
}
void wdLoop() {
    unsigned long ct = millis();
    if (wd_enabled && encoder_enabled
        && ((ct-wd_sample_time) > wd_last_time)) {
        encoderRead();
        wd_last_time = ct;
        for (int i=0;i<3;i++) {
            if (pids[i].Compute()) {
                wd_send = true;
                if (abs(output[i]-pids[i].GetSum()) < stability_threshold) {
                    if (stability_count[i] < 255) {
                        stability_count[i]++;
                    }
                } else {
                    stability_count[i] = 0;
                }
                if (output[i] > wd_min && output[i] < wd_max) {
                    emo_count[i] = 0;
                } else {
                    emo_count[i]++;
                }
                if (emo_count[i] > emo_count_threshold) {
                    wdStop();
                    return;
                }
                analogWrite(wd_pins[i], output[i]);
            }
        }
        if (logging && wd_send) {
            sendPIDLog();
        }
    }
}
bool wdStable() {
    for (int i=0;i<3;i++) {
        if (stability_count[i] < stability_count_threshold) {
            return false;
        }
    }
    return true;
}
void wdResetStable() {
    for (int i=0;i<3;i++) {
        stability_count[i] = 0;
    }
}
void wdStart() {
    wd_enabled = true;
}
void wdStop() {
    for (int i=3; i<3; i++) {
        if (wd_invert) {
            analogWrite(wd_pins[i], 255);
        } else {
            analogWrite(wd_pins[i], 0);
        }
        stability_count[i] = 0;
        emo_count[i] = 0;
    }
    wd_enabled = false;
}
bool wdEnabled() {
    return wd_enabled;
}

void sendPIDLog() {
    encoderStatus();
    usrMsg.sendCmdStart(kRecievePIDLogging);
    for (int i=0;i<3;i++) {
        usrMsg.sendCmdBinArg<double>(L_target[i]);
        usrMsg.sendCmdBinArg<double>(encoder_counts[i]);
        usrMsg.sendCmdBinArg<double>(output[i]);
        usrMsg.sendCmdBinArg<byte>(stability_count[i]);
        usrMsg.sendCmdBinArg<byte>(emo_count[i]);
        usrMsg.sendCmdBinArg<byte>(encoder_stats[i]);
    }
    usrMsg.sendCmdBinArg<bool>(encoder_enabled);
    usrMsg.sendCmdEnd();
    wd_send = false;
}
void setDrive() {
    if (usrMsg.readBinArg<bool>()) {
        wdStart();
    } else {
        wdStop();
    }
}
void getDrive() { usrMsg.sendBinCmd<bool>(kGetDrive, wd_enabled); }
void getPIDCoeff() {
    usrMsg.sendCmdStart(kGetPIDCoeff);
    for (int i=0;i<3;i++) {
        usrMsg.sendCmdBinArg<double>(pids[i].GetKp());
        usrMsg.sendCmdBinArg<double>(pids[i].GetKi());
        usrMsg.sendCmdBinArg<double>(pids[i].GetKd());
    }
    usrMsg.sendCmdEnd();
}
void setEMOCountThreshold() {
    emo_count_threshold = usrMsg.readBinArg<byte>();
}
void getEMOCountThreshold() {
    usrMsg.sendBinCmd<uint16_t>(
        kGetEMOCountThreshold, (uint16_t)emo_count_threshold);
}
void setStabilityCountThreshold() {
    stability_count_threshold = usrMsg.readBinArg<byte>();
}
void getStabilityCountThreshold() {
    usrMsg.sendBinCmd<uint16_t>(
        kGetStabilityCountThreshold, (uint16_t)stability_count_threshold);
}
void setStabilityThreshold() {
    stability_threshold = usrMsg.readBinArg<double>();
}
void getStabilityThreshold() {
    usrMsg.sendBinCmd<double>(kGetStabilityThreshold,
                              stability_threshold);
}
void getTimes() {
    unsigned long ct = millis();
    // ct-wd_sample_time) > wd_last_time
    usrMsg.sendCmdStart(kGetTimes);
    usrMsg.sendCmdBinArg<uint32_t>((uint32_t)ct);
    usrMsg.sendCmdBinArg<uint32_t>((uint32_t)wd_sample_time);
    usrMsg.sendCmdBinArg<uint32_t>((uint32_t)wd_last_time);
    usrMsg.sendCmdEnd();
}
