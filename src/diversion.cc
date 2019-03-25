#include "diversion.h"
#include <random>
#include <cstdlib>
#include "pyre.h"

namespace diversion {

Diversion::Diversion() {
    double divert_prob = 0.02;
    int divert_num = 1;
    int times_diverted = 0;
}

// @param divert_prob probability of diversion occuring at any point
// @param divert_num the number of diversions to occur in a scenario
// @param times_diverted number of diversions so far in simulation
bool divert(double divert_prob, int divert_num, 
    int times_diverted) {

    if (times_diverted < divert_num) {
        double seed = rng_gen(0,1);
        if (seed > divert_prob) {
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

// @param lower lower limit on random number
// @param upper upper limit on random number
double rng_gen(double lower, double upper) {
    sample = lower + (rand()/(RAND_MAX+1.0))*upper;
    return sample;
}

// @param mean average value
// @param sigma variance of value
double normal_dist(double mean, double sigma) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::normal_distribution<double> d(mean, sigma);
    sample = d(gen);
    return sample;
}
}