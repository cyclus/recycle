#ifndef RECYCLE_SRC_PYRE_REDUCTION_H_
#define RECYCLE_SRC_PYRE_REDUCTION_H_

#include "cyclus.h"
#include "process.h"

class Process;

namespace pyro {

/// The Reduct class describes the Electroreduction sub-process. The class
/// relies on the input current, lithium oxide weight percent, volume, and time.
/// Reduct depends on the Process base class for SepMaterial and parameter data.
/// Parameter corellations are found from a compilation of literature review.
class Reduct : public pyro::Process {

 public:
  /// @brief defaul constructor
  Reduct();
  /// @brief overloaded constructor
  Reduct(double reduct_current, double reduct_li2o, 
	double reduct_volume, double reduct_time);

 private:

  /// Coulombic efficiency coefficients
  std::vector<double> coul;
  /// Lithium oxide catalyst coefficients
  std::vector<double> lith;

  void SetCoeff();

  /// @brief Calculates the overall efficiency of the process by combining coulombic and catalyst effs.
  /// @return a value between 0 and 1 to be multiplied with separation efficiencies
  double Efficiency();

  /// @brief A function for the relationship between current and process efficiency
  /// @return a value between 0 and 1 relating to separation efficiency
  double Coulombic();

  /// @brief Relates the efficiency with respect to lithium oxide quantity
  /// @return a value between 0 and 1 relating to separation efficiency
  double Catalyst();

  double Throughput();
};
} // namespace pyro
#endif // RECYCLE_SRC_PYRE_REDUCTION_H_

