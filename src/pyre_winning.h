#ifndef RECYCLE_SRC_PYRE_WINNING_H_
#define RECYCLE_SRC_PYRE_WINNING_H_

#include "cyclus.h"
#include "process.h"
#include "recycle_version.h"

namespace recycle {

class Winning : protected Process {

public:

Winning();

Winning(double winning_current, double winning_time, 
	double winning_flowrate, double winning_volume);

/// @param feed feed yellowcake from voloxidation
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to fuel fabrication
cyclus::Material::Ptr WinningSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

private:

double Efficiency(std::vector<double> current, 
	std::vector<double> reprocess_time, std::vector<double> flowrate);

double Throughput(std::vector<double> reprocess_time, double volume);
};
}
#endif // RECYCLE_SRC_PYRE_WINNING_H_
