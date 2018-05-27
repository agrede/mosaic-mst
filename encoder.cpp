#include "encoder.h"

Encoders::Encoders() { }

void Encoders::begin() {
    for (int i=0; i<3; i++) {
        pinMode(select[i], OUTPUT);
        digitalWrite(select[i], HIGH);
    }
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        digitalWrite(select[i], LOW);
        SPI.transfer(0x88); // Write to MDR0
        SPI.transfer(0x03); // Configure x4 quadrature count
        digitalWrite(select[i], HIGH);
        delayMicroseconds(100); // Ensure new command
        digitalWrite(select[i], LOW);
        SPI.transfer(0x90); // Write to MDR1
        SPI.transfer(0x04); // Disable Counting
        digitalWrite(select[i], HIGH);
        delayMicroseconds(100); // Ensure new command
        digitalWrite(select[i], LOW);
        SPI.transfer(0x20); // Clear Counter
        digitalWrite(select[i], HIGH);
        delayMicroseconds(100); // Ensure new command
        digitalWrite(select[i], LOW);
        SPI.transfer(0x30); // Clear STR
        digitalWrite(select[i], HIGH);
    }
    SPI.endTransaction();
}
void Encoders::start() {
    enabled = true;
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        digitalWrite(select[i], LOW);
        SPI.transfer(0x90); // Write to MDR1
        SPI.transfer(0x00); // Enable Counting
        digitalWrite(select[i], HIGH);
    }
    SPI.endTransaction();
}
void Encoders::stop() {
    enabled = false;
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        digitalWrite(select[i], LOW);
        SPI.transfer(0x90); // Write to MDR1
        SPI.transfer(0x04); // Disable Counting
        digitalWrite(select[i], HIGH);
    }
    SPI.endTransaction();
}
void Encoders::clear() {
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        digitalWrite(select[i], LOW);
        SPI.transfer(0x20); // Clear Counter
        digitalWrite(select[i], HIGH);
    }
    SPI.endTransaction();
}
void Encoders::read() {
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        long cnt = 0;
        digitalWrite(select[i], LOW);
        SPI.transfer(0x60); // read from CNTR
        for (int j=0; j<4; j++) {
            cnt |= (SPI.transfer(0x00) << (3-j)*8);
        }
        digitalWrite(select[i], HIGH);
        counts[i] = double(cnt);
    }
    SPI.endTransaction();
}
void Encoders::status() {
    SPI.beginTransaction(SPISET);
    for (int i=0; i<3; i++) {
        digitalWrite(select[i], LOW);
        stats[i] = SPI.transfer(0x70); // read from STR
        digitalWrite(select[i], HIGH);
        delayMicroseconds(100); // Ensure new command
        digitalWrite(select[i], LOW);
        stats[i] = SPI.transfer(0x30); // clear STR
        digitalWrite(select[i], HIGH);
    }
    SPI.endTransaction();
}
