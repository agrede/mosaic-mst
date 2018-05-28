#include "errorcells.hpp"


double error_scale = 0.2;
bool error_enabled = false;
bool error_recieved = false;

void errorSetup() {
    errMsg.attach(kRecieveError, recieveError);
    usrMsg.attach(kSetErrorCell, setErrorCell);
    usrMsg.attach(kGetErrorCell, getErrorCell);
    usrMsg.attach(kSetErrorScale, setErrorScale);
    usrMsg.attach(kGetErrorScale, getErrorScale);
}
void errorLoop() {
    if (error_enabled && error_recieved) {
        error_recieved = false;
        errMsg.sendCmd(kRequestError);
    }
}

void recieveError() {
    for (int i=0; i<3; i++) {
        pos_vector[i] += error_scale*errMsg.readBinArg<double>();
    }
    setTarget();
    error_recieved = true;
}

void getErrorCell() {
    usrMsg.sendBinCmd<bool>(kGetErrorCell, error_enabled);
}
void setErrorCell() { error_enabled = usrMsg.readBinArg<bool>(); }
void setErrorScale() {
    error_scale = usrMsg.readBinArg<double>();
}
void getErrorScale() {
    usrMsg.sendBinCmd<double>(kGetErrorScale, error_scale);
}
