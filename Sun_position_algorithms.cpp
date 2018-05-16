#include "Sun_position_algorithms.h"

sunpos::sunpos(double h, int d, int m, int y, double dt, double Long, double lat, double P, double T) {
  UT = h;
  Day = d;
  Month = m;
  Year = y;
  Dt = dt;
  Longitude = Long;
  Latitude = lat;
  Pressure = P;
  Temperature = T;
}



void sunpos::Algorithm1(char flag) {

  if (Month <= 2) {
    mt = Month + 12;
    yt = Year - 1;
  } else {
    mt = Month;
    yt = Year;
  }

  t = double(int(365.25*double(yt-2000)) + int(30.6001*double(mt+1)) - int(0.01*double(yt)) + Day) + 0.0416667*UT - 21958.0;
  te = t + 1.1574e-5*Dt;

  wte = 0.017202786*te;

  s1 = sin(wte);
  c1 = cos(wte);
  s2 = 2.0*s1*c1;
  c2 = (c1+s1)*(c1-s1);

  RightAscension = -1.38880 + 1.72027920e-2*te + 3.199e-2*s1 - 2.65e-3*c1 + 4.050e-2*s2 + 1.525e-2*c2;
  RightAscension = fmod(RightAscension, PI2);
  if (RightAscension < 0.0) RightAscension += PI2;

  Declination = 6.57e-3 + 7.347e-2*s1 - 3.9919e-1*c1 + 7.3e-4*s2 - 6.60e-3*c2;

  HourAngle = 1.75283 + 6.3003881*t + Longitude - RightAscension;
  HourAngle = fmod(HourAngle + PI, PI2) - PI;
  if (HourAngle < -PI) HourAngle += PI2;

  if (flag == 's') return;

  sp = sin(Latitude);
  cp = sqrt((1-sp*sp));
  sd = sin(Declination);
  cd = sqrt(1-sd*sd);
  sH = sin(HourAngle);
  cH = cos(HourAngle);
  se0 = sp*sd + cp*cd*cH;
  ep = asin(se0) - 4.26e-5*sqrt(1.0-se0*se0);
  Azimuth = atan2(sH, cH*sp - sd*cp/cd);

  if (ep > 0.0)
    De = (0.08422*Pressure) / ((273.0+Temperature)*tan(ep + 0.003138/(ep + 0.08919)));
  else
    De = 0.0;

  Zenith = PIM - ep - De;

}



void sunpos::Algorithm2(char flag) {

  if (Month <= 2) {
    mt = Month + 12;
    yt = Year - 1;
  } else {
    mt = Month;
    yt = Year;
  }

  t = double(int(365.25*double(yt-2000)) + int(30.6001*double(mt+1)) - int(0.01*double(yt)) + Day) + 0.0416667*UT - 21958.0;
  te = t + 1.1574e-5*Dt;

  wte = 0.017202786*te;

  s1 = sin(wte);
  c1 = cos(wte);
  s2 = 2.0*s1*c1;
  c2 = (c1+s1)*(c1-s1);
  s3 = s2*c1 + c2*s1;
  c3 = c2*c1 - s2*s1;
  s4 = 2.0*s2*c2;
  c4 = (c2+s2)*(c2-s2);

  RightAscension = -1.38880 + 1.72027920e-2*te + 3.199e-2*s1 - 2.65e-3*c1 + 4.050e-2*s2 + 1.525e-2*c2 + 1.33e-3*s3 + 3.8e-4*c3 + 7.3e-4*s4 + 6.2e-4*c4;
  RightAscension = fmod(RightAscension, PI2);
  if (RightAscension < 0.0) RightAscension += PI2;

  Declination = 6.57e-3 + 7.347e-2*s1 - 3.9919e-1*c1 + 7.3e-4*s2 - 6.60e-3*c2 + 1.50e-3*s3 - 2.58e-3*c3 + 6e-5*s4 - 1.3e-4*c4;

  HourAngle = 1.75283 + 6.3003881*t + Longitude - RightAscension;
  HourAngle = fmod(HourAngle + PI, PI2) - PI;
  if (HourAngle < -PI) HourAngle += PI2;

  if (flag == 's') return;

  sp = sin(Latitude);
  cp = sqrt((1-sp*sp));
  sd = sin(Declination);
  cd = sqrt(1-sd*sd);
  sH = sin(HourAngle);
  cH = cos(HourAngle);
  se0 = sp*sd + cp*cd*cH;
  ep = asin(se0) - 4.26e-5*sqrt(1.0-se0*se0);
  Azimuth = atan2(sH, cH*sp - sd*cp/cd);

  if (ep > 0.0)
    De = (0.08422*Pressure) / ((273.0+Temperature)*tan(ep + 0.003138/(ep + 0.08919)));
  else
    De = 0.0;

  Zenith = PIM - ep - De;

}



void sunpos::Algorithm3(char flag) {

  if (Month <= 2) {
    mt = Month + 12;
    yt = Year - 1;
  } else {
    mt = Month;
    yt = Year;
  }

  t = double(int(365.25*double(yt-2000)) + int(30.6001*double(mt+1)) - int(0.01*double(yt)) + Day) + 0.0416667*UT - 21958.0;
  te = t + 1.1574e-5*Dt;

  wte = 0.0172019715*te;

  lambda = -1.388803 + 1.720279216e-2*te + 3.3366e-2*sin(wte - 0.06172) + 3.53e-4*sin(2.0*wte - 0.1163);

  epsi = 4.089567e-1 - 6.19e-9*te;

  sl = sin(lambda);
  cl = cos(lambda);
  se = sin(epsi);
  ce = sqrt(1-se*se);

  RightAscension = atan2(sl*ce, cl);
  if (RightAscension < 0.0)
    RightAscension += PI2;

  Declination = asin(sl*se);

  HourAngle = 1.7528311 + 6.300388099*t + Longitude - RightAscension;
  HourAngle = fmod(HourAngle + PI, PI2) - PI;
  if (HourAngle < -PI) HourAngle += PI2;

  if (flag == 's') return;

  sp = sin(Latitude);
  cp = sqrt((1-sp*sp));
  sd = sin(Declination);
  cd = sqrt(1-sd*sd);
  sH = sin(HourAngle);
  cH = cos(HourAngle);
  se0 = sp*sd + cp*cd*cH;
  ep = asin(se0) - 4.26e-5*sqrt(1.0-se0*se0);
  Azimuth = atan2(sH, cH*sp - sd*cp/cd);

  if (ep > 0.0)
    De = (0.08422*Pressure) / ((273.0+Temperature)*tan(ep + 0.003138/(ep + 0.08919)));
  else
    De = 0.0;

  Zenith = PIM - ep - De;

}



void sunpos::Algorithm4(char flag) {

  if (Month <= 2) {
    mt = Month + 12;
    yt = Year - 1;
  } else {
    mt = Month;
    yt = Year;
  }

  t = double(int(365.25*double(yt-2000)) + int(30.6001*double(mt+1)) - int(0.01*double(yt)) + Day) + 0.0416667*UT - 21958.0;
  te = t + 1.1574e-5*Dt;

  wte = 0.0172019715*te;

  L = 1.752790 + 1.720279216e-2*te + 3.3366e-2*sin(wte - 0.06172) + 3.53e-4*sin(2.0*wte - 0.1163);

  nu = 9.282e-4*te - 0.8;
  Dlam = 8.34e-5*sin(nu);
  lambda = L + PI + Dlam;

  epsi = 4.089567e-1 - 6.19e-9*te + 4.46e-5*cos(nu);

  sl = sin(lambda);
  cl = cos(lambda);
  se = sin(epsi);
  ce = sqrt(1-se*se);

  RightAscension = atan2(sl*ce, cl);
  if (RightAscension < 0.0)
    RightAscension += PI2;

  Declination = asin(sl*se);

  HourAngle = 1.7528311 + 6.300388099*t + Longitude - RightAscension + 0.92*Dlam;
  HourAngle = fmod(HourAngle + PI, PI2) - PI;
  if (HourAngle < -PI) HourAngle += PI2;

  if (flag == 's') return;

  sp = sin(Latitude);
  cp = sqrt((1-sp*sp));
  sd = sin(Declination);
  cd = sqrt(1-sd*sd);
  sH = sin(HourAngle);
  cH = cos(HourAngle);
  se0 = sp*sd + cp*cd*cH;
  ep = asin(se0) - 4.26e-5*sqrt(1.0-se0*se0);
  Azimuth = atan2(sH, cH*sp - sd*cp/cd);

  if (ep > 0.0)
    De = (0.08422*Pressure) / ((273.0+Temperature)*tan(ep + 0.003138/(ep + 0.08919)));
  else
    De = 0.0;

  Zenith = PIM - ep - De;

}



void sunpos::Algorithm5(char flag) {

  if (Month <= 2) {
    mt = Month + 12;
    yt = Year - 1;
  } else {
    mt = Month;
    yt = Year;
  }

  t = double(int(365.25*double(yt-2000)) + int(30.6001*double(mt+1)) - int(0.01*double(yt)) + Day) + 0.0416667*UT - 21958.0;
  te = t + 1.1574e-5*Dt;

  wte = 0.0172019715*te;

  s1 = sin(wte);
  c1 = cos(wte);
  s2 = 2.0*s1*c1;
  c2 = (c1+s1)*(c1-s1);
  s3 = s2*c1 + c2*s1;
  c3 = c2*c1 - s2*s1;

  L = 1.7527901 + 1.7202792159e-2*te + 3.33024e-2*s1 - 2.0582e-3*c1 + 3.512e-4*s2 - 4.07e-5*c2 + 5.2e-6*s3 - 9e-7*c3 -8.23e-5*s1*sin(2.92e-5*te) + 1.27e-5*sin(1.49e-3*te - 2.337) + 1.21e-5*sin(4.31e-3*te + 3.065) + 2.33e-5*sin(1.076e-2*te - 1.533) + 3.49e-5*sin(1.575e-2*te - 2.358) + 2.67e-5*sin(2.152e-2*te + 0.074) + 1.28e-5*sin(3.152e-2*te + 1.547) + 3.14e-5*sin(2.1277e-1*te - 0.488);

  nu = 9.282e-4*te - 0.8;
  Dlam = 8.34e-5*sin(nu);
  lambda = L + PI + Dlam;

  epsi = 4.089567e-1 - 6.19e-9*te + 4.46e-5*cos(nu);

  sl = sin(lambda);
  cl = cos(lambda);
  se = sin(epsi);
  ce = sqrt(1-se*se);

  RightAscension = atan2(sl*ce, cl);
  if (RightAscension < 0.0)
    RightAscension += PI2;

  Declination = asin(sl*se);

  HourAngle = 1.7528311 + 6.300388099*t + Longitude - RightAscension + 0.92*Dlam;
  HourAngle = fmod(HourAngle + PI, PI2) - PI;
  if (HourAngle < -PI) HourAngle += PI2;

  if (flag == 's') return;

  sp = sin(Latitude);
  cp = sqrt((1-sp*sp));
  sd = sin(Declination);
  cd = sqrt(1-sd*sd);
  sH = sin(HourAngle);
  cH = cos(HourAngle);
  se0 = sp*sd + cp*cd*cH;
  ep = asin(se0) - 4.26e-5*sqrt(1.0-se0*se0);
  Azimuth = atan2(sH, cH*sp - sd*cp/cd);

  if (ep > 0.0)
    De = (0.08422*Pressure) / ((273.0+Temperature)*tan(ep + 0.003138/(ep + 0.08919)));
  else
    De = 0.0;

  Zenith = PIM - ep - De;

}
