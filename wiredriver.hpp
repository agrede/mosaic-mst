#ifndef WIREDRIVER_HPP
#define WIREDRIVER_HPP

#include "Arduino.h"
#include "encoder.h"
#include "PID_v1.h"
#include <CmdMessenger.h>

class WireDriver{
public:
    encoders *enc;
    bool enable = false;
    PID pids[3];
    double min = 0.0;
    double max = 229.5;
    double stability_threshold = 2;
    byte stability_count_threshold = 10;
    bool logging = false;
    WireDriver(CmdMessenger *cmg; long (*targ)[3], double Kp[3], double Ki[3], double Kd[3]);
    void loop();
    bool stable();
    void startDriver();
    void stopDriver();
private:
    const int[3] pins = {3, 5, 10};
    const bool INVERT = false;
    CmdMessenger *cmdMsg;
    byte logCmd;
    bool last_enable = false;
    unsigned long period = 100;
    byte stability_count[3];
    double output[3];
    long (*target)[3];
}

#endif
