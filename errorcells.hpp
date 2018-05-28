#ifndef ERRORCELLS_HPP
#define ERRORCELLS_HPP
#include "Arduino.h"
#include "coms.hpp"
#include "movement.hpp"

extern double scale;
extern bool error_enabled;
extern bool error_recieved;

void errorSetup();
void errorLoop();

void recieveError();
void setErrorCell();
void getErrorCell();
void setErrorScale();
void getErrorScale();

#endif
