#ifndef RECYCLE_SRC_PYRE_REDUCTION_H_
#define RECYCLE_SRC_PYRE_REDUCTION_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

class Reduct {

public:

Reduct();

Reduct(double reduct_current,double reduct_li2o,double reduct_volume,double reduct_time);

/// @param feed feed yellowcake from voloxidation
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to refining
cyclus::Material::Ptr ReductSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

private:

double current;
double lithium_oxide;
double volume;
double reprocess_time;

double Efficiency(double current, double lithium_oxide);

double Throughput(double volume, double reprocess_time);
};
}
#endif // RECYCLE_SRC_PYRE_REDUCTION_H_
