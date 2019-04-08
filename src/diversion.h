#ifndef RECYCLE_SRC_DIVERSION_H_
#define RECYCLE_SRC_DIVERSION_H_

namespace recycle {

class Diversion {

public:

double divert_prob;
int divert_num, times_diverted;

Diversion();

Diversion(double divert_prob, int divert_num);

// @param divert_prob probability of diversion occuring at any point
// @param divert_num the number of diversions to occur in a scenario
// @param times_diverted number of diversions so far in simulation
bool divert(double divert_prob, int divert_num, int times_diverted);

// @param lower lower limit on random number
// @param upper upper limit on random number
double rng_gen(double lower, double upper);

// @param mean average value
// @param sigma variance of value
double normal_dist(double mean, double sigma);

void set_prob(double input);

void set_num(int input);

void set_times_divert(int input);

double get_prob();

int get_num();

int get_times_divert();
};
}
#endif // RECYCLE_SRC_DIVERSION_H_
