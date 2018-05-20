/* C++ header file  Sun_position_algorithms.h



last modified : 9 Sept. 2014

List of modifications:
9 Sept. 2014: since function fmod returns also negative values on some compilers, a check was added after using it



This file contains the 5 algorithms to calculate the sun position described in the paper "Five new algorithms for the computation of sun position from 2010 to 2110", Solar Energy 86 (2012) 1323-1337.


Use of the algortihms:

- This header file must be included in the program.

- At the beginning, before starting calculations, a class of type sunpos must be declared. The constructor of the class accepts also initial values for the input parameters of the algorithms, but they can be omitted (default values are used) and given later.

Examples of declarations:

  sunpos Sun_Position;

or

  sunpos Sun_Position(UT, Day, Month, Year, Dt, Long, Lat, P, T);

In this second example, variables UT, Month, etc. show the parameters that can be given to the constructor: UT is the time UT (double, in hours), Day, Month and Year are the date (all integers), Dt is the difference TT-UT (double, in seconds), Long and Lat are the geographical longitude and latitude (double, in radians), P is the pressure (double, in atm), T is the temperature (double, in Celsius degrees).

- Before every position calculations, the input data can be set simply assigning new values to the members of the class: es. to change the hour UT in the class declared as above, assigning the value NewUT, the command is

  Sun_Position.UT = NewUT;

Input members in the class are named UT, Day, Month, Year, Dt, Longitude, Latitude, Pressure, Temperature.
The variables not assigned before the calculation are left unchanged, This is convenient, since usually changes in the input data concern only the time, while coordinates are kept fixed. Parameters left unchanged can be declared at the beginning, e.g. using the constructor, and then left unchanged through all the computations.

- once the input variables are set, the calculations (e.g. using Algorithm 3) can be performed simply with the command

  Sun_Position.Algorithm3();

or

  Sun_Position.Algorithm3('s');

In the first case the long algorithm is used, while in the second case (with the parameter 's') the short algorithm is used.
Other algorithms (1,2,4,5) can be used simply changing the final number.
This command does not return any values, but writes the output values computed in the corresponding class members, which can be read and used as usual variables. In order to output on the screen the value of the Zenith, for example, the command

  cout<<Sun_Position.Zenith<<endl;

can be used. Output members in the class are named RightAscension, Declination, HourAngle, Zenith, Azimuth.

Note that all the class members are public; no access control to variables, neither consistency controls are made, since the maximum computational efficiency and versatility are the aim. A bit of caution should be employed (e.g. when changing input parameters, outputs does not change automatically and remains at the old values until the function AlgorithmN is called; or, when using short algorithms, quantities not computed (Zenith and Azimuth) are left with the previous values).

The following code performs the computation of the position at every hour (from 1 to 24) on 1st January 2012, at Rome, using Algorithm 5, writing a table of the zenith and azimuth on the output screen. All the input parameters except UT are left unchanged as declared in the constructor, while the hour is changed at every calculation.

  (...)
  sunpos Sun_Position(0.0,	// initial hour (arbitrary - not used)
		      1,1,2012,	// date
		      65.0,	// Dt
		      0.21787,	// Longitude in radians
		      0.73117,	// Latitude in radians
		      1, 20);	// Pressure (atm) and Temperature (Celsius)
  cout<<endl<<"Hour\tZen\tAz"<<endl;
  for (double hour = 1.0; hour < 24.1; hour += 1.0) {
    Sun_Position.UT = hour-1.0; 	// correct UT ("hour" is the time of Rome)
    Sun_Position.Algorithm5();
    cout<<hour<<"\t"<<Sun_Position.Zenith<<"\t"<<Sun_Position.Azimuth<<endl;
  }
  (...)

*/


//#include<cmath.h>
// AR: Arduino library does not support cmath.h library
#ifndef SUNPOS_H
#define SUNPOS_H
#include "Arduino.h"

#ifndef PI
#define PI 3.14159265358979
#endif

#define PI2 6.28318530717959 // 2*PI
#define PIM 1.57079632679490 // PI/2

class sunpos {
public:

  // input data:
  double UT;
  int Day;
  int Month;
  int Year;
  double Dt;
  double Longitude;
  double Latitude;
  double Pressure;
  double Temperature;

  //output data
  double RightAscension;
  double Declination;
  double HourAngle;
  double Zenith;
  double Azimuth;

  //auxiliary
  double t, te, wte, s1, c1, s2, c2, s3, c3, s4, c4,
    sp, cp, sd, cd, sH, cH, se0, ep, De, lambda, epsi,
    sl, cl, se, ce, L, nu, Dlam;
  int yt, mt;

  // constructor
  sunpos(double h = 0.0, int  d = 1, int m = 1, int y = 2010,
    double Dt = 65.0, double Long = 0.0, double lat = 0.0,
    double P = 1.0, double T = 20.0);

  // algorithms
  void Algorithm1(char flag = 'l');
  void Algorithm2(char flag = 'l');
  void Algorithm3(char flag = 'l');
  void Algorithm4(char flag = 'l');
  void Algorithm5(char flag = 'l');

};




#endif
