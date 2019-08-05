#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "process.h"
#include "boost/function.hpp"

class Process;

namespace recycle {

/// The Refine class describes the Electrorefiner sub-process. The class
/// relies on the input temperature, pressure, rotation, batch size, and time.
/// Refine depends on the Process base class for SepMaterial and parameter data.
/// Parameter corellations are found from a compilation of literature review.
class Refine : public Process {

public:

  Refine();

  Refine(double refine_temp, double refine_press, double refine_rotation, 
	double refine_batch_size, double refine_time);

  /// @brief Convergence condition for the bisect function, used in each subprocess
  /// @return boolean if answer is within 1E-6 of true value
  struct TerminationCondition;

private:

  /// Thermal Efficiency coefficients - Lee et al.
  double t0_;
  double t1_;
  double t2_;
  double t3_;
  /// Pressure Efficiency coefficients
  double p0_;
  double p1_;
  double p2_;
  double p3_;
  /// Stirrer Efficiency coefficient
  double a0_;
  double a1_;
  double a2_;
  double a3_;

  /// @brief sets all coefficients to their defaults
  void SetCoeff();

  /// @brief If type is operator, function passes location and siphon to OpDivertMat
  /// @param type Type of diversion to take place - Operator or Nefarious
  /// @param location location of diversion as a pair of ("subprocess"."parameter")
  /// @param siphon how much material will be diverted
  void DivertMat(std::string type, std::pair<std::string, std::string> location,
    double siphon);

  /// @brief Determines what parameter to change to increase subprocess efficiency for diversion
  /// @param location location of diversion as a pair of ("subprocess","parameter")
  /// @param siphon how much material will be diverted
  void OpDivertMat(std::pair<std::string, std::string> location, double siphon);

  /// @brief Uses boost bisect to find the root of a given function
  /// @param SolveFunc a boost binded unary function whose root is needed
  /// @param lower lower bound for the root - by default 10% lower than current val
  /// @param upper upper bound for the root - by default 10% higher than current val
  /// @return root of SolveFunc
  double Bisector(boost::function<double(double)> SolveFunc, double lower, double upper);

  /// @brief a boost binded version of the Thermal function whose only parameter is temperature
  boost::function<double(double)> ThermalFunc;

  /// @brief Calculates the overall efficiency of the process by combining thermal, pressure and stirrer effs.
  /// @return a value between 0 and 1 to be multiplied with separation efficiencies
  double Efficiency();

  /// @brief A function describing the Thermal Efficiency of the process
  /// @param temp The operating temperature of the process
  /// @param newEff the new efficiency due to diversion, used to solve for new temperature
  /// @return a value between 0 and 1 to be used in the Efficiency function
  double Thermal(double temp, double newEff);

  /// @brief overloaded version of Thermal that uses getters for all parameters
  double Thermal();

  /// @brief A function describing the Pressure Efficiency of the process
  /// @param prs The operating pressure of the electrorefiner
  /// @param newEff the new efficiency due to diversion, used to solve for new pressure
  /// @return a value between 0 and 1 to be used in the Efficiency function
  double PressureEff(double prs, double newEff);

  /// @brief overloaded version of PressureEff that uses getters for all parameters
  double PressureEff();

  /// @brief A function describing the Central Stirrer Efficiency of the process
  /// @param rot The operating speed of the central stirrer
  /// @param newEff the new efficiency due to diversion, used to solve for new rotation speed
  /// @return a value between 0 and 1 to be used in the Efficiency function
  double Agitation(double rot, double newEff);

  /// @brief overloaded version of Agitation that uses getters for all parameters
  double Agitation();

  /// @brief Determines the throughput of the process based on facility parameters
  /// @return material throughput kg / day
  double Throughput();

  /// @return the value for thermal efficiency coefficient 0
  double t0();

  /// @return the value for thermal efficiency coefficient 1
  double t1();

  /// @return the value for thermal efficiency coefficient 2
  double t2();

  /// @return the value for thermal efficiency coefficient 3
  double t3();

  /// @return the value for pressure efficiency coefficient 0
  double p0();

  /// @return the value for pressure efficiency coefficient 1
  double p1();

  /// @return the value for pressure efficiency coefficient 2
  double p2();

  /// @return the value for pressure efficiency coefficient 3
  double p3();

  /// @return the value for rotation efficiency coefficient 0
  double a0();

  /// @return the value for rotation efficiency coefficient 1
  double a1();

  /// @return the value for rotation efficiency coefficient 2
  double a2();

  /// @return the value for rotation efficiency coefficient 3
  double a3();
};
}
#endif // RECYCLE_SRC_PYRE_REFINING_H_

