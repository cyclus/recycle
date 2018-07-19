#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "recycle_version.h"

/// @param feed salt with uranium and fission product feed
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to electrowinning
cyclus::Material::Ptr RefineSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);


#endif // RECYCLE_SRC_PYRE_REFINING_H_
