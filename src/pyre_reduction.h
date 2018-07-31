#ifndef RECYCLE_SRC_PYRE_REDUCTION_H_
#define RECYCLE_SRC_PYRE_REDUCTION_H_

#include "cyclus.h"
#include "recycle_version.h"

class Reduct {
/// @param feed feed yellowcake from voloxidation
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to refining
cyclus::Material::Ptr ReductionSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

};
#endif // RECYCLE_SRC_PYRE_REDUCTION_H_
