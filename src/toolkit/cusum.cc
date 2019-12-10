#include "cusum.h"
#include "pyre.h"

namespace recycle {

Cusum::Cusum() {
    this.magnitude = 0.5;
    this.threshold = 4;
    this.startup = 10;
    this.old_cusum = 0;
}

Cusum::Cusum(double magnitude, double threshold, int startup) {
    this.magnitude = magnitude;
    this.threshold = threshold;
    this.startup = startup;
    this.old_cusum = 0;
    this.count = 0;
}

void UpdateTest(double new_val) {
    int count = count + 1;

    double mean;
    double new_mean = mean + (new_val - mean) / count;
    double variance = variance + (new_val - mean) * (new_val - new_mean);
    double mean = new_mean;
    double std = sqrt(variance);

    double delta = magnitude * std;
    double lambda = threshold * std;

    cusum_val = max(0, old_cusum + (new_val - mean - delta));

    if(count > startup) {
        this.divert = cusum_val > lambda;
    }

    old_cusum = cusum_val;
}

bool Flag() {
    return this.divert;
}
} // namespace recycle

