#ifndef RECYCLE_SRC_PROCESS_H_
#define RECYCLE_SRC_PROCESS_H_

#include "cyclus.h"
#include "pyre.h"
#include <vector>

using cyclus::Material;

namespace recycle {

class Process {

public:

Process();

void temp(double new_temp); // setter
double temp(); // getter

void current(double new_current);
double current();

void rotation(double new_rotate);
double rotation();

void pressure(double new_press);
double pressure();

void lithium(double new_lith);
double lithium();

void Rtime(double new_time);
double Rtime();

void flowrate(double new_flow);
double flowrate();

void b_size(double new_size);
double b_size();

void volume(double new_vol);
double volume();

// This returns an untracked material that should just be used for
// its composition and qty - not in any real inventories, etc.
Material::Ptr SepMaterial(std::map<int, double> effs, Material::Ptr mat);

virtual void DivertMat(std::string type, std::pair<std::string, std::string> location,
  double siphon);

virtual double Efficiency();

std::map<std::string, std::vector<double>> subcomponents;

};
}
#endif // RECYCLE_SRC_PROCESS_H_
