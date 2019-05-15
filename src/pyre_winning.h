#ifndef RECYCLE_SRC_PYRE_WINNING_H_
#define RECYCLE_SRC_PYRE_WINNING_H_

#include "cyclus.h"
#include "process.h"
#include "recycle_version.h"

class Process;

namespace recycle {

class Winning : public recycle::Process {

public:

Winning();

Winning(double winning_current, double winning_time, 
	double winning_flowrate, double winning_volume);

private:

double c0,c1,c2,c3,c4,t0,t1,r0,r1;

void set_coeff();

double Efficiency();

double Coulombic(double c0, double c1, double c2, double c3, double c4);

double Temporal(double c0, double c1);

double RateEff(double c0, double c1);

double Throughput();
};
}
#endif // RECYCLE_SRC_PYRE_WINNING_H_
