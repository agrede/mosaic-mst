#include "encoder.hpp"

double encoder_counts[3] = {0.0, 0.0, 0.0};
bool encoder_enabled = false;
byte encoder_stats[3];

void encoderSetup() {
    for (int i=0; i<3; i++) {
        pinMode(encoder_select[i], OUTPUT);
        digitalWrite(encoder_select[i], HIGH);
    }
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        digitalWrite(encoder_select[i], LOW);
        SPI.transfer(0x88); // Write to MDR0
        SPI.transfer(0x03); // Configure x4 quadrature count
        digitalWrite(encoder_select[i], HIGH);
        delayMicroseconds(100); // Ensure new command
        digitalWrite(encoder_select[i], LOW);
        SPI.transfer(0x90); // Write to MDR1
        SPI.transfer(0x04); // Disable Counting
        digitalWrite(encoder_select[i], HIGH);
        delayMicroseconds(100); // Ensure new command
        digitalWrite(encoder_select[i], LOW);
        SPI.transfer(0x20); // Clear Counter
        digitalWrite(encoder_select[i], HIGH);
        delayMicroseconds(100); // Ensure new command
        digitalWrite(encoder_select[i], LOW);
        SPI.transfer(0x30); // Clear STR
        digitalWrite(encoder_select[i], HIGH);
    }
    SPI.endTransaction();
    usrMsg.attach(kSetEncoder, setEncoder);
    usrMsg.attach(kGetEncoder, getEncoder);
    usrMsg.attach(kSetEncoderCenter, setEncoderCenter);
    usrMsg.attach(kGetEncoderCounts, getEncoderCounts);
    usrMsg.attach(kGetEncoderStats, getEncoderStatus);
}

bool encoderStart() {
    encoder_enabled = true;
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        digitalWrite(encoder_select[i], LOW);
        SPI.transfer(0x90); // Write to MDR1
        SPI.transfer(0x00); // Enable Counting
        digitalWrite(encoder_select[i], HIGH);
        delayMicroseconds(100); // Ensure new command
        digitalWrite(encoder_select[i], LOW);
        if (!encoder_enabled && SPI.transfer(0x48) == 0x00) {
            encoder_enabled = false;
        }
        digitalWrite(encoder_select[i], HIGH);
    }
    SPI.endTransaction();
    return encoder_enabled;
}
bool encoderStop() {
    encoder_enabled = false;
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        digitalWrite(encoder_select[i], LOW);
        SPI.transfer(0x90); // Write to MDR1
        SPI.transfer(0x04); // Disable Counting
        digitalWrite(encoder_select[i], HIGH);
        // delayMicroseconds(100); // Ensure new command
        // digitalWrite(encoder_select[i], LOW);
        // if (encoder_enabled || SPI.transfer(0x48) != 0x04) {
        //     encoder_enabled = true;
        // }
        // digitalWrite(encoder_select[i], HIGH);
    }
    SPI.endTransaction();
    return !encoder_enabled;
}
void encoderClear(int index) {
    SPI.beginTransaction(SPISET);
    if (index < 0){
        for (int i=0; i<3; i++) {
            digitalWrite(encoder_select[i], LOW);
            SPI.transfer(0x20); // Clear Counter
            digitalWrite(encoder_select[i], HIGH);
        }
    } else {
        digitalWrite(encoder_select[index], LOW);
        SPI.transfer(0x20); // Clear Counter
        digitalWrite(encoder_select[index], HIGH);
    }
    SPI.endTransaction();
}
void encoderRead() {
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        long cnt = 0;
        digitalWrite(encoder_select[i], LOW);
        SPI.transfer(0x60); // read from CNTR
        for (int j=0; j<4; j++) {
            cnt |= (SPI.transfer(0x00) << (3-j)*8);
        }
        digitalWrite(encoder_select[i], HIGH);
        encoder_counts[i] = double(cnt);
    }
    SPI.endTransaction();
}
void encoderStatus() {
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        digitalWrite(encoder_select[i], LOW);
        encoder_stats[i] = SPI.transfer(0x70); // read from STR
        digitalWrite(encoder_select[i], HIGH);
        delayMicroseconds(100); // Ensure new command
        digitalWrite(encoder_select[i], LOW);
        encoder_stats[i] = SPI.transfer(0x30); // clear STR
        digitalWrite(encoder_select[i], HIGH);
    }
    SPI.endTransaction();
}
void getEncoder() {
    usrMsg.sendBinCmd<bool>(kGetEncoder, encoder_enabled); }
void setEncoder() {
    if (usrMsg.readBinArg<bool>()) {
        encoderStart();
    } else {
        encoderStop();
    }
}
void getEncoderCounts() {
    if (encoder_enabled) {
        encoderRead();
        usrMsg.sendCmdStart(kGetEncoderCounts);
        for (int i=0;i<3;i++) {
            usrMsg.sendCmdBinArg<double>(encoder_counts[i]);
        }
        usrMsg.sendCmdEnd();
    } else {
        usrMsg.sendCmdStart(kGetEncoderCounts);
        for (int i=0;i<3;i++) {
            usrMsg.sendCmdBinArg<double>(-555.0);
        }
        usrMsg.sendCmdEnd();
    }
}
void getEncoderStatus() {
    encoderStatus();
    usrMsg.sendCmdStart(kGetEncoderStats);
    for (int i=0;i<3;i++) {
        usrMsg.sendCmdBinArg<byte>(encoder_stats[i]);
    }
    usrMsg.sendCmdEnd();
}
void setEncoderCenter() {
    int index = usrMsg.readBinArg<int>();
    if (index > -2 && index < 3) {
        encoderClear(index);
    }
}
