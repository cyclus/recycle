#ifndef RECYCLE_SRC_PYRE_WINNING_H_
#define RECYCLE_SRC_PYRE_WINNING_H_

#include "cyclus.h"
#include "process.h"

class Process;

namespace pyro {

/// The Winning class describes the Electrowinning sub-process. The class
/// relies on the input current, flowrate, and time.
/// Winning depends on the Process base class for SepMaterial and parameter data.
/// Parameter corellations are found from a compilation of literature review.
class Winning : public pyro::Process {

 public:

  /// @brief default constructor
  Winning();

  /// @brief overloaded constructor
  Winning(double winning_current, double winning_time, 
	double winning_flowrate, double winning_volume);

 private:
  /// The coefficients are derived from the following:
  /// 1 M. M. Saleh, J. W. Weidner, B. E. El-Anadouli, and B. G. Ateya, "Electrowinning of Nonnoble Metals 
  ///   with Simultaneous Hydrogen Evolution at Flow-Through Porous Electrodes III. Time Effects," 
  ///   J. Electrochem. Soc., vol. 144, no. 3, pp. 922-927, Mar. 1997.
  /// 2 T.-J. Kim, G.-Y. Kim, D. Yoon, D.-H. Ahn, and S. Paek, "Development of an anode structure 
  ///   consisting of graphite tubes and a SiC shroud for the electrowinning process in 
  ///   molten salt," Journal of Radioanalytical and Nuclear Chemistry, vol. 295, no. 3, 
  ///   pp. 1855-1859, Mar. 2013.


  /// @brief Coulombic Efficiency coefficients
  std::vector<double> coul = {-0.00685,0.20413,-2.273,11.2046,-19.7493};
  /// @brief Temporal Efficiency coefficients
  std::vector<double> ti = {0.2903, -1.696};
  /// @brief Flowrate Efficiency coefficients
  std::vector<double> r = {0.12435,0.7985};

  /// @brief Calculates the overall efficiency of the process by combining coulombic, temporal and flowrate effs.
  /// @return a value between 0 and 1 to be multiplied with separation efficiencies
  double Efficiency();

  /// @brief A function for the relationship between current and process efficiency
  /// @return a value between 0 and 1 relating to separation efficiency
  double Coulombic();

  /// @brief This function describes how much material can be separated in the allotted time.
  /// @return a value between 0 and 1 relating to separation efficiency
  double Temporal();

  /// @brief Material separation as a function of flowrate
  /// @return a value between 0 and 1 relating to separation efficiency
  double RateEff();

  /// @brief This function determines the throughput of the given subprocess
  double Throughput();
};
} // namespace pyro
#endif // RECYCLE_SRC_PYRE_WINNING_H_
