#include "movement.hpp"

double pos_vector[3] = {0.0, 0.0, 0.0};
bool pos_updated = false;
double L_target[3];
double L_0[3] = {0.0, 0.0, 0.0};

void setTarget() {
    for (int i=0; i<3; i++) {
        pos_vector[i] = constrain(pos_vector[i], -pos_limits[i], pos_limits[i]);
    }
    for (int i=0; i<3; i++) {
        L_target[i] = wire_resolution*(
            sqrt(
                sq(x_frame[i]+pos_vector[2]*y_sheet[i]
                   -x_sheet[i]-pos_vector[0])
                +sq(y_frame[i]-pos_vector[2]*x_sheet[i]
                    -y_sheet[i]-pos_vector[1])) - L_0[i]);
    }
    pos_updated = true;
}

void setL0() {
    for (int i=0; i<3; i++) {
        L_0[i] = (sqrt(
                      sq(x_frame[i]-x_sheet[i])
                      +sq(y_frame[i]-y_sheet[i])));
    }
}
void userSetTarget() {
    for (int i=0;i<3;i++) {
        pos_vector[i] = usrMsg.readBinArg<double>();
    }
    setTarget();
}
void userGetTarget() {
    usrMsg.sendCmdStart(kGetTarget);
    for (int i=0;i<3;i++) {
        usrMsg.sendCmdBinArg<double>(pos_vector[i]);
    }
    usrMsg.sendCmdEnd();
}
void movementSetup() {
    setL0();
    usrMsg.attach(kSetTarget, userSetTarget);
    usrMsg.attach(kGetTarget, userGetTarget);
}
