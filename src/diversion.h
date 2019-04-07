#ifndef RECYCLE_SRC_DIVERSION_H_
#define RECYCLE_SRC_DIVERSION_H_

#include "pyre.h"
#include "cyclus.h"

namespace recycle {

class Diversion {

// @param divert_prob probability of diversion occuring at any point
// @param divert_num the number of diversions to occur in a scenario
// @param times_diverted number of diversions so far in simulation
bool divert(double divert_prob, int divert_num, 
    int times_diverted);

// @param lower lower limit on random number
// @param upper upper limit on random number
double rng_gen(double lower, double upper);

// @param mean average value
// @param sigma variance of value
double normal_dist(double mean, double sigma);

};
}
#endif // RECYCLE_SRC_DIVERSION_H_
