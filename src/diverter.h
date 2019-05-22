#ifndef RECYCLE_SRC_DIVERTER_H_
#define RECYCLE_SRC_DIVERTER_H_

#include "cyclus.h"
#include "process.h"

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
int frequency;
/// @param divert_number how many times diversion should take place within a scenario
int divert_number;
/// @param divert_times keeps track how many times diversion has taken place
int divert_times;
/// @param quantity how much material will be siphoned off at diversion
double quantity;
std::string type;

Diverter();

Diverter(std::pair<std::string,std::string> location, 
    int frequency, double quantity, int divert_number, std::string type);

void locate(std::pair<std::string,std::string> new_local);
std::pair<std::string,std::string> locate();

void freq(int new_freq);
int freq();

void siphon(double new_siphon);
double siphon();

void divert_num(int new_num);
int divert_num();

void divert_time(int new_time);
int divert_time();

/// @brief Divert passes diversion information to the appropriate sub-process if it is
/// the correct time step.
/// @param t the current time step
/// @param components a map of class objects for each sub-process
/// @return true if diversion takes place and false if not
bool Divert(int t, std::map<std::string, Process> components);

/// @brief DivertStream uses ExtractQty to remove material from separations streams and places into
/// a "diverted" stream.
/// @param sepstreams a map of all streams
/// @return the new diverted stream
Material::Ptr DivertStream(std::map<std::string, Material::Ptr> sepstreams);

};
}
#endif // RECYCLE_SRC_DIVERTER_H_
