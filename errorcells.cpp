#include "errorcells.hpp"


ErrorCells::ErrorCells(CmdMessenger *cmsg, double (*offst)[3], byte rqstCmd) {
    cmdMsg = cmsg;
    offsets = offst;
    recieved = true;
    requestCmd = rqstCmd;
}
void ErrorCells::loop(){
    if (recieved && enabled) {
        recieved = false;
        cmdMsg->sendCmd(requestCmd);
    }
}
void ErrorCells::updateOffsets() {
    for (int i=0;i<3;i++) {
        *offsets[i] += scale*(cmdMsg->readBinArg<double>());
    }
    setTarget();
    recieved = true;
}
