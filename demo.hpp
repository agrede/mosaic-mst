#ifndef DEMO_HPP
#define DEMO_HPP

#include "Arduino.h"
#include "coms.hpp"
#include "movement.hpp"

const int demo_steps = 30;
const double demo_R = 7.0;
extern bool demo_enabled;


void demoSetup();
void demoLoop();

void setDemo();
void getDemo();

#endif
