#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "process.h"
#include "recycle_version.h"

namespace recycle {

class Refine : public Process {

public:

Refine();

Refine(double refine_temp, double refine_press, double refine_rotation, 
	double refine_batch_size, double refine_time);

/// @param feed salt with uranium and fission product feed
/// @param stream the separation efficiency for reduction streams
/// @return composition composition of the separated material sent to electrowinning
cyclus::Material::Ptr RefineSepMaterial(std::map<int, double> effs,
	cyclus::Material::Ptr mat);

private:

double Efficiency(std::vector<double> temp, std::vector<double> pressure, 
	std::vector<double> rotation);

double Throughput(std::vector<double> b_size, 
	std::vector<double> reprocess_time);
};
}
#endif // RECYCLE_SRC_PYRE_REFINING_H_

