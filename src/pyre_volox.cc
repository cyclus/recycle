#include "pyre.h"
#include "pyre_volox.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

namespace recycle {

// kdh : a default constructor should construct a valid object.
Volox::Volox() {
  temp(0);
  Rtime(0);
  flowrate(0);
  volume(0);
}

// kdh : these should just be temp, time, flowrate
// and all associated pragmas should have defaults...
Volox::Volox(double temp, 
             double Rtime, 
             double flowrate, 
             double volume
        ) 
        {
          temp(temp);
          Rtime(Rtime);
          flowrate(flowrate);
          volume(volume); 
}

double Volox::Efficiency() {
  return Thermal()*Temporal()*RateEff();
}

//KDH instead of double thermal = .... (above)
double Volox::Thermal(double c0 = 4.369E-9,
                      double c1 = -1.0833E-5, 
                      double c2 = 0.008069,
                      double c3 = -0.9726
) {
  // KDH document this function so that it's clear what paper you got the relation from
  return c0*pow(temp(), 3) + c1*pow(temp(),2) + c2*temp() + c3;
}

double Volox::Temporal(double c0 = 0.2903,
                       double c1 = 1.696
) {
  return c0 * log(Rtime()*3600) - c1;
}

double Volox::RateEff(double c0 = 0.12435,
                      double c1 = 0.7985
) {
  return c0*log(flowrate()) + c1;
}

double Volox::Throughput() {
  // placeholder calculation
  // KDH no need to call it volox_*, as we know we are in the volox scope
  return volume() / flowrate()*Rtime();
}
}
