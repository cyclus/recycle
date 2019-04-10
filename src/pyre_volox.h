#ifndef RECYCLE_SRC_PYRE_VOLOX_H_
#define RECYCLE_SRC_PYRE_VOLOX_H_

#include "process.h"
#include "pyre.h"
#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

class Volox : public recycle::Process {

public:

Volox();

Volox(double volox_temp, double volox_time, 
	double volox_flowrate, double volox_volume);

private:

/// @param temp temperature of the volox process
/// @param time time spent in the process
/// @param flow mass flow rate
/// @return efficiency separation efficiency of the voloxidation process
double Efficiency();

double Thermal(double c0, double c1, double c2, double c3);

double Temporal(double c0, double c1);

double RateEff(double c0, double c1);

/// @return throughput material throughput of voloxidation
double Throughput();

};
}
#endif // RECYCLE_SRC_PYRE_VOLOX_H_
