#include "encoder.h"

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
        delayMicroseconds(1); // Ensure new command
        digitalWrite(select[i], LOW);
        SPI.transfer(0x90); // Write to MDR1
        SPI.transfer(0x04); // Disable Counting
        digitalWrite(select[i], HIGH);
        delayMicroseconds(1); // Ensure new command
        digitalWrite(select[i], LOW);
        SPI.transfer(0x20); // Clear Counter
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
        counts[i] = 0;
        digitalWrite(select[i], LOW);
        SPI.transfer(0xE4); // Load CNTR to OTR
        digitalWrite(select[i], HIGH);
        delayMicroseconds(1); // Ensure new command
        digitalWrite(select[i], LOW);
        SPI.transfer(0x68); // read from OTR
        for (int j=0; j<4; j++) {
            counts[i] |= (SPI.transfer(0x00) << (3-j)*8);
        }
        digitalWrite(select[i], HIGH);
    }
    SPI.endTransaction();
}
void Encoders::center() {
    clear();
    stop();
}
