#ifndef ERRORCELLS_HPP
#define ERRORCELLS_HPP

#include "Arduino.h"
#include <CmdMessenger.h>

class ErrorCells{
public:
    ErrorCells(CmdMessenger *cmsg, float (*offst)[3]);
    void loop();
    void updateOffsets();
private:
    bool recieved;
    CmdMessenger *cmdMsg;
    float (*offsets)[3];
    byte requestCmd;
}

#endif
