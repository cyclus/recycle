#ifndef RECYCLE_SRC_PYRE_REDUCTION_H_
#define RECYCLE_SRC_PYRE_REDUCTION_H_

#include "cyclus.h"
#include "process.h"

class Process;

namespace recycle {

/// The Reduct class describes the Electroreduction sub-process. The class
/// relies on the input current, lithium oxide weight percent, volume, and time.
/// Reduct depends on the Process base class for SepMaterial and parameter data.
/// Parameter corellations are found from a compilation of literature review.
class Reduct : public recycle::Process {

public:

Reduct();

Reduct(double reduct_current, double reduct_li2o, 
	double reduct_volume, double reduct_time);

private:

/// Coulombic efficiency coefficients
double c0;
double c1;
double c2;
double c3;
double c4;
/// Lithium oxide catalyst coefficients
double ca0;
double ca1;

void set_coeff();

/// @brief Calculates the overall efficiency of the process by combining coulombic and catalyst effs.
/// @return a value between 0 and 1 to be multiplied with separation efficiencies
double Efficiency();

/// @brief A function for the relationship between current and process efficiency
/// @return a value between 0 and 1 relating to separation efficiency
double Coulombic(double c0, double c1, double c2, double c3, double c4);

/// @brief Relates the efficiency with respect to lithium oxide quantity
/// @return a value between 0 and 1 relating to separation efficiency
double Catalyst(double c0, double c1);

double Throughput();
};
}
#endif // RECYCLE_SRC_PYRE_REDUCTION_H_

