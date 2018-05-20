#ifndef WIREDRIVER_HPP
#define WIREDRIVER_HPP

#include "Arduino.h"
#include "encoder.h"
#include "PID_v1.h"
#include <CmdMessenger.h>

class WireDriver{
public:
    Encoders *enc;
    PID *pids[3];
    double min = 0.0;
    double max = 229.5;
    double stability_threshold = 2;
    byte stability_count_threshold = 20;
    byte emo_count_threshold = 60;
    bool logging = false;
    WireDriver(Encoders *ec, CmdMessenger *cmg, long (*targ)[3], byte lCmd, double Kp[3], double Ki[3], double Kd[3]);
    void loop();
    void begin();
    bool stable();
    void resetStable();
    void startDriver();
    void stopDriver();
    bool enabled();
private:
    const int pins[3] = {3, 5, 10};
    const bool INVERT = false;
    bool enable = false;
    bool last_enable = false;
    CmdMessenger *cmdMsg;
    byte logCmd;
    unsigned long period = 100;
    byte stability_count[3];
    byte emo_count[3];
    double output[3];
    long (*target)[3];
};

#endif
