#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "process.h"
#include "recycle_version.h"

namespace recycle {

class Refine : public Process {

public:

Refine();

Refine(double refine_temp, double refine_press, double refine_rotation, 
	double refine_batch_size, double refine_time);

private:

double Efficiency();

double Thermal(double c0, double c1, double c2, double c3);

double PressureEff(double c0, double c1, double c2, double c3);

double Agitation(double c0, double c1, double c2, double c3);

double Throughput();
};
}
#endif // RECYCLE_SRC_PYRE_REFINING_H_

