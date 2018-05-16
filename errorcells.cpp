#include "errorcells.hpp"


ErrorCells::ErrorCells(CmdMessenger *cmsg, double (*offst)[3]) {
    cmdMsg = cmsg;
    offsets = offst;
    recieved = true;
}
void ErrorCells::loop(){
    if (recieved && enabled) {
        recieved = false;
        cmdMsg->sendCmd(requestCmd);
    }
}
void ErrorCells::updateOffsets() {
    for (int i=0;i<3;i++) {
        *offsets[i] += scale*(cmdMsg->readDoubleArg());
    }
    recieved = true;
}
void ErrorCells::start(){
    enabled = true;
}
void ErrorCells::stop(){
    enabled = false;
}
