#ifndef RECYCLE_SRC_PYRE_VOLOX_H_
#define RECYCLE_SRC_PYRE_VOLOX_H_

#include "cyclus.h"
#include "recycle_version.h"


class Volox {
public:
	Volox::Volox(volox_temp, volox_time, volox_flowrate, volox_volume);
/// @param feed feed snf
/// @param stream the separation efficiency of voloxidation
/// @return composition composition of the resulting product and waste
cyclus::Material::Ptr VoloxSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

};

/// @param temp temperature of the volox process
/// @param time time spent in the process
/// @param flow mass flow rate
/// @return efficiency separation efficiency of the voloxidation process
double Volox::Efficiency(volox_temp, volox_time, volox_flowrate);

/// @return throughput material throughput of voloxidation
double Volox::Through(volox_flowrate, volox_time, volox_volume);

#endif // RECYCLE_SRC_PYRE_VOLOX_H_
