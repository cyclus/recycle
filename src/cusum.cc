#include "cusum.h"
#include "pyre.h"

namespace recycle {

Cusum::Cusum() {
    this.magnitude = 0.5;
    this.threshold = 4;
    this.startup = 10;
}

Cusum::Cusum(double magnitude, double threshold, int startup) {
    this.magnitude = magnitude;
    this.threshold = threshold;
    this.startup = startup;
}

void update_test(double newVal) {
    int count = count + 1;

    double newMean = mean + (newVal - mean) / count;
    double variance = variance + (newVal - mean)*(newVal - newMean);
    double mean = newMean;
    double std = sqrt(variance);

    double delta = magnitude * std;
    double lambda = threshold * std;

    cusum_val = max(0, oldCusum + (newVal - mean - delta));

    if(count > startup) {
        this.divert = cusum_val > lambda;
    }

    oldCusum = cusum_val;
}

bool flag() {
    return divert;
}
}
}
