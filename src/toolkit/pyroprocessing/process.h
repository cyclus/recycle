#ifndef RECYCLE_SRC_PROCESS_H_
#define RECYCLE_SRC_PROCESS_H_

#include <vector>
#include "cyclus.h"

using cyclus::Material;

namespace pyro {

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

  /// @param new_temp update temperature
  void temp(double new_temp);
  /// @return temperature
  double temp();

  /// @param new_current update current
  void current(double new_current);
  /// @return current
  double current();

  /// @param new_rotate updates rotation speed
  void rotation(double new_rotate);
  /// @return rotation speed
  double rotation();

  /// @param new_press update pressure
  void pressure(double new_press);
  /// @return pressure
  double pressure();

  /// @param new_lith update lithium oxide percent
  void lithium(double new_lith);
  /// @return lithium oxide percentage
  double lithium();

  /// @param new_time update how much time spent processing
  void Rtime(double new_time);
  /// @return time spent in processing
  double Rtime();

  /// @param new_flow update material flowrate
  void flowrate(double new_flow);
  /// @return material flowrate
  double flowrate();

  /// @param new_size update batch size
  void b_size(double new_size);
  /// @return batch size
  double b_size();

  /// @param new_vol update valume of the process
  void volume(double new_vol);
  /// @return volume of the process
  double volume();

};
} // namespace recycle
#endif // RECYCLE_SRC_PROCESS_H_
