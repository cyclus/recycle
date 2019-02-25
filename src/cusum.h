#ifndef RECYCLE_SRC_CUSUM_H
#define RECYCLE_SRC_CUSUM_H

#include "pyre.h"
#include "cyclus.h"

namespace recycle {

class Cusum {

private:
double oldCusum = 0;
double cusum_val;
double mean;
double newMean;
double newVal;
double variance;
double magnitude;
double threshold;

int startup;
int count;

bool divert;

public:
// default constructor
Cusum();

// overloaded constructor
Cusum(double magnitude, double threshold, int startup);

void update_test(double newVal);

bool flag();
}
}