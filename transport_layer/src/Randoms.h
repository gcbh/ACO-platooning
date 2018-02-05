//
//  Randoms.h
//  uw.fydo.aco
//
//  Created by Geoffrey Heath on 2017-06-06.
//
//

#ifndef Randoms_h
#define Randoms_h

#include <cstdlib>
#include <cmath>
#include <limits>

class Randoms {
public:
    Randoms(long x);
    double Normal(double avg, double sigma);
    double Uniforme ();
    double sorte (int m);
    float  ran1(long *idum);
    float  gaussdev(long *idum);
private:
    long xpto;
    
};

#endif /* Randoms_h */
