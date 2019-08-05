#ifndef RECYCLE_SRC_CUSUM_H_
#define RECYCLE_SRC_CUSUM_H_

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