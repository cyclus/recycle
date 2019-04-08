#include "diversion.h"
#include "pyre.h"

namespace recycle {

Diversion::Diversion() {}

Diversion::Diversion(
    double divert_prob = 0.02,
    int divert_num = 1
) {
    set_prob(divert_prob);
    set_num(divert_num);
    set_times_divert(0);
}

bool Diversion::divert(double divert_prob, int divert_num, 
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

double Diversion::rng_gen(double lower, double upper) {
    double sample = lower + (rand()/(RAND_MAX+1.0))*upper;
    return sample;
}

double Diversion::normal_dist(double mean, double sigma) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::normal_distribution<double> d(mean, sigma);
    double sample = d(gen);
    return sample;
}

void Diversion::set_prob(double input) {
    divert_prob = input;
}

void Diversion::set_num(int input) {
    divert_num = input;
}

void Diversion::set_times_divert(int input) {
    times_diverted = input;
}

double Diversion::get_prob() {
    return divert_prob;
}

int Diversion::get_num() {
    return divert_num;
}

int Diversion::get_times_divert() {
    return times_diverted;
}
}
