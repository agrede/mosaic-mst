#include "errorcells.hpp"

class ErrorCells{
public:
    ErrorCells(CmdMessenger *cmsg, float (*offst)[3]) {
        cmdMsg = cmsg;
        offsets = offst;
        recieved = true;
    }
    void loop(){
        if (recieved) {
            recieved = false;
            *cmdMsg.sendCmd(requestCmd);
        }
    }
    void updateOffsets() {
        for (int i=0;i<3;i++) {
            *offsets[i] = *cmdMsg.readFloatArg();
        }
        recieved = true
    }
}
