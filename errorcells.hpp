#ifndef ERRORCELLS_HPP
#define ERRORCELLS_HPP

#include "Arduino.h"
#include <CmdMessenger.h>

class ErrorCells{
public:
    float scale = 0.2;
    bool enabled = false;
    ErrorCells(CmdMessenger *cmsg, double (*offst)[3]);
    void loop();
    void updateOffsets();
    void start();
    void stop();
private:
    bool recieved;
    CmdMessenger *cmdMsg;
    double (*offsets)[3];
    byte requestCmd;
};

#endif
