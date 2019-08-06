#ifndef RECYCLE_SRC_PYRE_WINNING_H_
#define RECYCLE_SRC_PYRE_WINNING_H_

#include "cyclus.h"
#include "process.h"

class Process;

namespace recycle {

/// The Winning class describes the Electrowinning sub-process. The class
/// relies on the input current, flowrate, and time.
/// Winning depends on the Process base class for SepMaterial and parameter data.
/// Parameter corellations are found from a compilation of literature review.
class Winning : public recycle::Process {

 public:

  Winning();

  Winning(double winning_current, double winning_time, 
	double winning_flowrate, double winning_volume);

 private:

  /// @brief Coulombic Efficiency coefficients
  std::vector<double> coul;
  /// @brief Temporal Efficiency coefficients
  std::vector<double> ti;
  /// @brief Flowrate Efficiency coefficients
  std::vector<double> r;

  void SetCoeff();

  /// @brief Calculates the overall efficiency of the process by combining coulombic, temporal and flowrate effs.
  /// @return a value between 0 and 1 to be multiplied with separation efficiencies
  double Efficiency();

  /// @brief A function for the relationship between current and process efficiency
  /// @return a value between 0 and 1 relating to separation efficiency
  double Coulombic(double c0, double c1, double c2, double c3, double c4);

  /// @brief This function describes how much material can be separated in the allotted time.
  /// @return a value between 0 and 1 relating to separation efficiency
  double Temporal(double c0, double c1);

  /// @brief Material separation as a function of flowrate
  /// @return a value between 0 and 1 relating to separation efficiency
  double RateEff(double c0, double c1);

  double Throughput();
};
} // namespace recycle
#endif // RECYCLE_SRC_PYRE_WINNING_H_
