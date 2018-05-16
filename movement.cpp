#include "movement.h"

double pos_vector[3] = {0.0, 0.0, 0.0};
long L_target[3];
long L_0[3];

void setTarget(double pos[3]) {
    for (int i=0; i<3; i++) {
        pos_vector[i] = constrain(pos[i], -pos_limits[i], pos_limits[i]);
    }
    for (int i=0; i<3; i++) {
        L_target[i] = long(
            wire_resolution*sqrt(
                sq(x_frame[i]+pos_vector[2]*y_sheet[i]
                    -x_sheet[i]-pos_vector[0])
                +sq(y_frame[i]+pos_vector[2]*x_sheet[i]
                     -y_sheet[i]-pos_vector[1]))) - L_0[i];
    }
}

void setL0() {
    for (int i=0; i<3; i++) {
        L_0[i] = long(
            wire_resolution*sqrt(
                sq(x_frame[i]-x_sheet[i])+sq(y_frame[i]-y_sheet[i])));
    }
}
