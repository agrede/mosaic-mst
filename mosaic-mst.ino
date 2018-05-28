#include <SPI.h>
#include "PID_v1.h"
#include "coms.hpp"
#include "encoder.hpp"
#include "movement.hpp"
#include "wiredriver.hpp"
#include "spotposition.hpp"
#include "errorcells.hpp"


void setup() {
    SPI.begin();
    comsSetup();
    encoderSetup();
    movementSetup();
    wdSetup();
    spotSetup();
    errorSetup();
    sendPIDLog();
}

void loop() {
    comsLoop();
    if (wdEnabled()) {
        if (wdStable()) {
            errorLoop();
        }
        wdLoop();
    }
    delayMicroseconds(100); // Ensure new command
}
