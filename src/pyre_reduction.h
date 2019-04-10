#ifndef RECYCLE_SRC_PYRE_REDUCTION_H_
#define RECYCLE_SRC_PYRE_REDUCTION_H_

#include "cyclus.h"
#include "process.h"
#include "recycle_version.h"

namespace recycle {

class Reduct : public Process {

public:

Reduct();

Reduct(double reduct_current, double reduct_li2o, 
	double reduct_volume, double reduct_time);

private:

double Efficiency();

double Coulombic(double c0, double c1, double c2, double c3, double c4);

double Catalyst(double c0, double c1)

double Throughput();
};
}
#endif // RECYCLE_SRC_PYRE_REDUCTION_H_

