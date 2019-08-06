#ifndef RECYCLE_SRC_PYRE_VOLOX_H_
#define RECYCLE_SRC_PYRE_VOLOX_H_

#include "process.h"
#include "cyclus.h"

class Process;

namespace recycle {

class Volox : public recycle::Process {

 public:

  Volox();

  Volox(double volox_temp, double volox_time, 
	double volox_flowrate, double volox_volume);

 private:

  /// @brief Temperature Coefficients
  std::vector<double> th;
  /// @brief Time Coefficients
  std::vector<double> ti;
  /// @brief Flowrate Coefficients
  std::vector<double> r;

  /// @brief sets the coefficients for each relationship
  void SetCoeff();

  /// @brief The Efficiency function combines the efficiencies of the temp, time, and flowrate params.
  /// @return efficiency separation efficiency of the voloxidation process
  double Efficiency();

  /// @brief Efficiency as a function of process temperature.
  /// @return a value between 0 and 1 relating to separation efficiency
  double Thermal(double c0, double c1, double c2, double c3);

  /// @brief This function describes how much material can be separated in the allotted time.
  /// @return a value between 0 and 1 relating to separation efficiency
  double Temporal(double c0, double c1);

  /// @brief Material separation as a function of flowrate
  /// @return a value between 0 and 1 relating to separation efficiency
  double RateEff(double c0, double c1);

  /// @return throughput material throughput of voloxidation
  double Throughput();

};
} // namespace recycle
#endif // RECYCLE_SRC_PYRE_VOLOX_H_
