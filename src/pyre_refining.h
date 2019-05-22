#ifndef RECYCLE_SRC_PYRE_REFINING_H_
#define RECYCLE_SRC_PYRE_REFINING_H_

#include "cyclus.h"
#include "process.h"
#include "recycle_version.h"
#include "boost/function.hpp"

class Process;

namespace recycle {

/// The Refining class describes the Electrorefiner sub-process. The class
/// relies on the input temperature, pressure, rotation, batch size, and time.
/// Refine depends on the Process base class for SepMaterial and parameter data.
/// Parameter corellations are found from a compilation of literature review.
class Refine : public Process {

public:

Refine();

Refine(double refine_temp, double refine_press, double refine_rotation, 
	double refine_batch_size, double refine_time);

private:

/// Thermal Efficiency coefficients - Lee et al.
double t0;
double t1;
double t2;
double t3;
/// Pressure Efficiency coefficients
double p0;
double p1;
double p2;
double p3;
/// Stirrer Efficiency coefficients
double a0;
double a1;
double a2;
double a3;

/// @brief sets all coefficients to their defaults
void set_coeff();

struct TerminationCondition;

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

/// @brief a boost binded version of the Thermal function whose only parameter is temperature
boost::function<double(double)> ThermalFunc;

/// @brief Calculates the overall efficiency of the process by combining thermal, pressure and stirrer effs.
/// @return a value between 0 and 1 to be multiplied with separation efficiencies
double Efficiency();

/// @brief A function describing the Thermal Efficiency of the process
/// @param temp The operating temperature of the process
/// @param new_eff the new efficiency due to diversion, used to solve for new temperature
/// @param c0 c0 through c3 are coefficients compiled from lit review 
/// @return a value between 0 and 1 to be used in the Efficiency function
double Thermal(double temp, double new_eff, double c0, double c1, double c2, double c3);


/// @brief A function describing the Pressure Efficiency of the process
/// @param c0 c0 through c3 are coefficients compiled from lit review 
/// @return a value between 0 and 1 to be used in the Efficiency function
double PressureEff(double c0, double c1, double c2, double c3);

/// @brief A function describing the Central Stirrer Efficiency of the process
/// @param c0 c0 through c3 are coefficients compiled from lit review 
/// @return a value between 0 and 1 to be used in the Efficiency function
double Agitation(double c0, double c1, double c2, double c3);

/// @brief Determines the throughput of the process based on facility parameters
/// @return material throughput kg / day
double Throughput();
};
}
#endif // RECYCLE_SRC_PYRE_REFINING_H_

