#include "diversion.h"
#include "pyre.h"

namespace diversion {

Diversion::Diversion() {
    double divert_prob = 0.02;
    int divert_num = 1;
    int times_diverted = 0;
}

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

double rng_gen(double lower, double upper) {
    sample = lower + (rand()/(RAND_MAX+1.0))*upper;
    return sample;
}

double normal_dist(double mean, double sigma) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::normal_distribution<double> d(mean, sigma);
    sample = d(gen);
    return sample;
}
}
