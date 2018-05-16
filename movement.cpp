#include "Arduino.h"
#include <movement.h>
#include <math.h>

float pos_vector[3] = {0.0, 0.0, 0.0};

void setTarget(float pos[3]) {
    for (int i=0; i<3; i++) {
        pos_vector[i] = constrain(pos[i], -pos_limits[i], pos_limits[i]);
    }
    for (int i=0; i<3; i++) {
        L_target[i] = long(
            wire_resolution*sqrt(
                sq(xframe[i]+pos_vector[2]*ysheet[i]
                    -xsheet[i]-pos_vector[0])
                +sq(yframe[i]+pos_vector[2]*xsheet[i]
                     -ysheet[i]-pos_vector[1]))) - L_0[i];
    }
}

void setL0() {
    for (int i=0; i<3; i++) {
        L_0[i] = long(
            wire_resolution*sqrt(
                sq(xframe[i]-xsheet[i])+sq(yframe[i]-ysheet[i])));
    }
}
