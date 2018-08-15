#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "recycle_version.h"

namespace recycle {

class Refine{

double temperature;
double pressure;
double rotation;
double batch_size;
double reprocess_time;

public:

Refine();

Refine(double refine_temp, double refine_press, double refine_rotation, double refine_batch_size, double refine_time);

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
