#ifndef RECYCLE_SRC_PYRE_WINNING_H_
#define RECYCLE_SRC_PYRE_WINNING_H_

#include "cyclus.h"
#include "recycle_version.h"

class Winning {
public:
/// @param feed feed yellowcake from voloxidation
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to fuel fabrication
cyclus::Material::Ptr WinningSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

};
#endif // RECYCLE_SRC_PYRE_WINNING_H_
