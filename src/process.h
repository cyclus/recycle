#ifndef RECYCLE_SRC_PROCESS_H_
#define RECYCLE_SRC_PROCESS_H_

#include "cyclus.h"
#include <vector>

using cyclus::Material;

namespace recycle {

/// The Process class is the base class for all sub-processes in PyRe.
/// The class is responsible for getting and storing process parameters, 
/// separating streams and setting diversion type.
class Process {

public:

  Process();

  /// SepMaterial returns a material object that represents the composition and
  /// quantity resulting from the separation of material from mat using the given
  /// mass-based efficiencies.  Each key in effs represents a nuclide or element
  /// (canonical PyNE form), and each value is the corresponding mass-based
  /// separations efficiency for that nuclide or element.  Note that this returns
  /// an untracked material that should only be used for its composition and qty
  /// - not in any real inventories, etc.
  Material::Ptr SepMaterial(std::map<int, double> effs, Material::Ptr mat);

  /// @brief This function will be replaced by DivertMat in each subprocess.
  virtual void DivertMat(std::string type, std::pair<std::string, std::string> location,
    double siphon);

  /// @brief This function will be replaced by DivertMat in each subprocess.
  virtual double Efficiency();

  std::map<std::string, std::vector<double>> subcomponents;

  /// @param new_temp
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

};
}
#endif // RECYCLE_SRC_PROCESS_H_
