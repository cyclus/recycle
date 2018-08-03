#ifndef RECYCLE_SRC_PYRE_REDUCTION_H_
#define RECYCLE_SRC_PYRE_REDUCTION_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

class Reduct {
public:

Reduct();

Reduct(reduct_current,reduct_li2o,reduct_volume,reduct_time);
/// @param feed feed yellowcake from voloxidation
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to refining
cyclus::Material::Ptr ReductionSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

double Efficiency(double current, double lithium_oxide);

double Throughput(double volume, double reprocess_time);

};
}
#endif // RECYCLE_SRC_PYRE_REDUCTION_H_
