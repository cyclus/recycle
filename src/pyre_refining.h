#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

class Refine{
public:

Refine();

Refine(refine_temp,refine_press,refine_rotation,refine_batch_size,refine_time);

/// @param feed salt with uranium and fission product feed
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to electrowinning
cyclus::Material::Ptr RefineSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

double Efficiency(double temperature, double pressure, double rotation);

double Throughput(double batch_size, double reprocess_time);

};
}
#endif // RECYCLE_SRC_PYRE_REFINING_H_
