#include <SPI.h>
#include "PID_v1.h"
#include "coms.hpp"
#include "encoder.hpp"
#include "movement.hpp"
#include "wiredriver.hpp"
#include "spotposition.hpp"
#include "errorcells.hpp"
#include "demo.hpp"


void setup() {
    SPI.begin();
    comsSetup();
    encoderSetup();
    movementSetup();
    wdSetup();
    spotSetup();
    errorSetup();
    sendPIDLog();
    demoSetup();
}

void loop() {
    comsLoop();
    if (wdEnabled()) {
        if (pos_updated) {
            wdResetStable();
            pos_updated = false;
        }
        if (wdStable()) {
            errorLoop();
            demoLoop();
        }
        wdLoop();
    }
    delayMicroseconds(100); // Ensure new command
}
