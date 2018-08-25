#include <SPI.h>
// #include "PID_v1.h"
#include "coms.hpp"
//#include "encoder.hpp"
#include "movement.hpp"
#include "motor.hpp"
#include "spotposition.hpp"
#include "errorcells.hpp"
#include "demo.hpp"


void setup() {
    SPI.begin();
    comsSetup();
    // encoderSetup();
    movementSetup();
    // wdSetup();
    smSetup();
    spotSetup();
    // errorSetup();
    // sendPIDLog();
    demoSetup();
}

void loop() {
    comsLoop();
    // if (wdEnabled()) {
    if (sm_enabled) {
        if (pos_updated) {
            // wdResetStable();
            sm_stable = false;
            pos_updated = false;
        }
        if (sm_stable) {
            // errorLoop();
            demoLoop();
        }
        // wdLoop();
        smLoop();
    }
    delayMicroseconds(100); // Ensure new command
}
