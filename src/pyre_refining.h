#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "process.h"
#include "recycle_version.h"
#include "boost/function.hpp"

class Process;

namespace recycle {

class Refine : public Process {

public:

Refine();

Refine(double refine_temp, double refine_press, double refine_rotation, 
	double refine_batch_size, double refine_time);

private:

double t0,t1,t2,t3,p0,p1,p2,p3,a0,a1,a2,a3,agi;

void set_coeff();

struct TerminationCondition;

void DivertMat(std::string type, std::pair<std::string, std::string> location,
  double siphon);

void OpDivertMat(std::pair<std::string, std::string> location, double siphon);

boost::function<double(double)> ThermalFunc;

double Efficiency();

double Thermal(double temp, double new_eff, double c0, double c1, double c2, double c3);

double PressureEff(double c0, double c1, double c2, double c3);

double Agitation(double c0, double c1, double c2, double c3);

double Throughput();
};
}
#endif // RECYCLE_SRC_PYRE_REFINING_H_

