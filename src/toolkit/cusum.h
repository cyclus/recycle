#ifndef RECYCLE_SRC_CUSUM_H_
#define RECYCLE_SRC_CUSUM_H_

#include "pyre.h"
#include "cyclus.h"

namespace recycle {

/// The Cusum class handles diversion detection using the Cumulative Sum method. 
/// The class also holds nonproliferation data such as
/// variance, threshold for diversion, and magnitude of diversion.
class Cusum {

 public:
  /// @brief default constructor
  Cusum();

  /// @brief Overloaded constructor
  Cusum(double magnitude, double threshold, int startup);

 private:
  /// @param oldCusum the starting value for the Cusum method
  double old_cusum;
  /// @param cusum_val the updated Cusum value
  double cusum_val; 
  double magnitude;
  /// @param threshold number of standard deviations before flag is raised
  double threshold;
  /// @param startup number of datapoints required before acceptable error
  int startup;
  /// @param count the number of data points collected
  int count;
  /// @param divert true if diversion is detected
  bool divert;

  /// @brief This function runs the Cusum algorithm with an updated data point
  /// @param newVal Next data point for the parameter of interest
  /// @return cusum_val the updated cusum value
  void UpdateTest(double newVal);

  /// @brief alarms if diversion has taken place
  bool Flag();
};
}
#endif // RECYCLE_SRC_CUSUM_H_
