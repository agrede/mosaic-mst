#include "errorcells.hpp"

class ErrorCells{
public:
    float scale = 0.2;
    float enabled = false;
    ErrorCells(CmdMessenger *cmsg, float (*offst)[3]) {
        cmdMsg = cmsg;
        offsets = offst;
        recieved = true;
    }
    void loop(){
        if (recieved && enabled) {
            recieved = false;
            *cmdMsg.sendCmd(requestCmd);
        }
    }
    void updateOffsets() {
        for (int i=0;i<3;i++) {
            *offsets[i] += scale*(*cmdMsg.readFloatArg());
        }
        recieved = true
    }
    void start(){
        enabled = true;
    }
    void stop(){
        enabled = false;
    }
}
