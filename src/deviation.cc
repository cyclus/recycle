#include "deviation.h"
#include <random>
#include "pyre.h"

namespace deviation {

double rng_gen(double lower, double upper) {
    sample = lower + (rand()/(RAND_MAX+1.0))*upper;
    return sample;
}

double NormalDist(double mean, double sigma) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::normal_distribution<double> d(mean, sigma);
    sample = d(gen);
    return sample;
}
}