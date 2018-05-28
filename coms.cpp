#include "coms.hpp"

CmdMessenger usrMsg = CmdMessenger(Serial);
CmdMessenger errMsg = CmdMessenger(Serial1);

bool logging = false;


void unknownCommand(){
    Serial.println("Unknown!");
}

void unknownCommandErr(){
    Serial1.println("Unknown!");
}

void getLogging() {
    usrMsg.sendBinCmd<bool>(kGetLogging, logging);
}
void setLogging() { logging = usrMsg.readBinArg<bool>(); }

void comsSetup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    usrMsg.attach(unknownCommand);
    errMsg.attach(unknownCommandErr);
    usrMsg.attach(kGetLogging, getLogging);
    usrMsg.attach(kSetLogging, setLogging);
}

void comsLoop() {
    usrMsg.feedinSerialData();
    errMsg.feedinSerialData();
}
