#include "demo.hpp"

bool demo_enabled = false;
double demo_step;
int demo_count = 0;

void demoSetup() {
    demo_step = 2.0*PI/double(demo_steps);
    usrMsg.attach(kSetDemo, setDemo);
    usrMsg.attach(kGetDemo, getDemo);
}

void demoLoop() {
    if (!demo_enabled) { return; }
    demo_count = (demo_count + 1) % demo_steps;
    double th = demo_step*demo_count;
    double th_int = PI/3.0*double(int(3.0*th/PI));
    double th_slp = th_int + PI/6.0;
    double x = cos(th);
    double y = -((x-cos(th_int))/tan(th_slp)-sin(th_int));
    pos_vector[0] = demo_R*x;
    pos_vector[1] = demo_R*y;
    pos_vector[2] = 0.0;
    setTarget();
}

void setDemo() {
    bool dm_en = usrMsg.readBinArg<bool>();
    if (dm_en != demo_enabled) {
        demo_enabled = dm_en;
        demo_count = 0;
    }
}
void getDemo() {
    usrMsg.sendBinCmd<bool>(kGetDemo, demo_enabled);
}
