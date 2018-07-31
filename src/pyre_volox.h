#ifndef RECYCLE_SRC_PYRE_VOLOX_H_
#define RECYCLE_SRC_PYRE_VOLOX_H_

#include "cyclus.h"
#include "recycle_version.h"


class Volox {
public:
/// @param feed feed snf
/// @param stream the separation efficiency of voloxidation
/// @return composition composition of the resulting product and waste
cyclus::Material::Ptr VoloxSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

};

#endif // RECYCLE_SRC_PYRE_VOLOX_H_
