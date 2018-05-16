#include <SPI.h>
#include "PID_v1.h"
#include <SoftwareSerial.h>
#include <CmdMessenger.h>
#include "encoder.h"
#include "movement.h"
#include "wiredriver.hpp"
#include "Sun_position_algorithms.h"
#include "errorcells.hpp"

CmdMessenger userMsg = CmdMessenger(Serial);
SoftwareSerial errSerial(9, 2); //rx, tx
CmdMessenger errMsg = CmdMessenger(errSerial);

ErrorCells errCells;
Encoders encoders;
