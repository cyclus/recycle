#ifndef RECYCLE_SRC_DIVERTER_H_
#define RECYCLE_SRC_DIVERTER_H_

#include "pyroprocessing/process.h"
#include "cyclus.h"

namespace recycle {

/// The Diverter class handles when to call diversion, and points to the appropriate
/// class for the location of diversion. The class also holds the diversion data such as
/// location, frequency, and how many times to divert.
class Diverter {

 public:

  cyclus::Context* tc;
  /// @param location where diversion will take place given by sub-process and parameter
  std::pair<std::string, std::string> location;
  /// @param frequency how many time steps take place between diversions 1e299 means no diversion
  int frequency = -1;
  /// @param divert_number how many times diversion should take place within a scenario
  int divert_number = 1;
  /// @param divert_times keeps track how many times diversion has taken place
  int divert_times;
  /// @param quantity how much material will be siphoned off at diversion
  double quantity = 0.01;
  /// @param type The type of diversion occuring: Operator or Nefarious
  std::string type = "operator";

  Diverter();

  Diverter(std::pair<std::string,std::string> location, 
    int frequency, double quantity, int divert_number, std::string type);

  /// @param new_local new location for diversion
  void locate(std::pair<std::string,std::string> new_local);
  /// @return location for diversion
  std::pair<std::string,std::string> locate();

  /// @param new_freq new frequency of diversion
  void freq(int new_freq);
  /// @return frequency of diversion
  int freq();

  /// @param new_siphon update material siphoned on diversion
  void siphon(double new_siphon);
  /// @return percent of material to be siphoned
  double siphon();

  /// @param new_num set how many times diversion should take place
  void divert_num(int new_num);
  /// @return max number of diversions
  int divert_num();

  /// @param new_time update number of diversion taken place
  void divert_time(int new_time);
  /// @return how many diversion have taken place in the sim
  int divert_time();

  /// @brief Divert passes diversion information to the appropriate sub-process if it is
  /// the correct time step.
  /// @param t the current time step
  /// @param components a map of class objects for each sub-process
  /// @return true if diversion takes place and false if not
  bool Divert(int t, std::map<std::string, pyro::Process*> components);

  /// @brief DivertStream uses ExtractQty to remove material from separations streams and places into
  /// a "diverted" stream.
  /// @param sepstreams a map of all streams
  /// @return the new diverted stream
  std::map<std::string, Material::Ptr> DivertStream(std::map<std::string, Material::Ptr> sepstreams);

};
} // namespace recycle
#endif // RECYCLE_SRC_DIVERTER_H_
