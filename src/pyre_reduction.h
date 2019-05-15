#ifndef RECYCLE_SRC_PYRE_REDUCTION_H_
#define RECYCLE_SRC_PYRE_REDUCTION_H_

#include "cyclus.h"
#include "process.h"
#include "recycle_version.h"

class Process;

namespace recycle {

class Reduct : public recycle::Process {

public:

Reduct();

Reduct(double reduct_current, double reduct_li2o, 
	double reduct_volume, double reduct_time);

private:

double c0,c1,c2,c3,c4,ca0,ca1;

void set_coeff();

double Efficiency();

double Coulombic(double c0, double c1, double c2, double c3, double c4);

double Catalyst(double c0, double c1);

double Throughput();
};
}
#endif // RECYCLE_SRC_PYRE_REDUCTION_H_

